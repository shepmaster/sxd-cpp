#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document.h"
#include "tokenizer.h"

Document::Document() :
  root_(nullptr),
  dict(intern_new()),
  managed_node_count_(0)
{
}

Document::~Document()
{
  intern_free(dict);
}

Element *
Document::new_element(const char * const name)
{
  Element *e = new Element(this, name);
  managed_node_count_++;
  return e;
}

TextNode *
Document::new_text_node(const char * const text)
{
  return new TextNode(this, text);
}

Comment *
Document::new_comment(const char * const text)
{
  return new Comment(this, text);
}

const char *
Document::intern(const char * const string)
{
  return intern_intern(dict, string);
}

unsigned int
Document::managed_node_count()
{
  return managed_node_count_;
}

void
Document::stop_managing_node(Node *node)
{
  managed_node_count_--;
}

void
Document::manage_node(Node *node)
{
  node->change_document(this);
  managed_node_count_++;
}

GQuark
document_parse_error_quark(void)
{
  return g_quark_from_static_string("document-parse-error-quark");
}

static void
_info_abort(GError **error, const char *file, int line)
{
  g_set_error(error, DOCUMENT_PARSE_ERROR, DOCUMENT_PARSE_ERROR_FAILED, "Aborting parsing at %s:%d\n", file, line);
}
#define info_abort(error)                       \
  _info_abort(error, __FILE__, __LINE__)

/* static char * */
/* print_context(tokenizer_t *tokenizer) */
/* { */
/*   tokenizer_context_t context; */
/*   GString *msg; */

/*   msg = g_string_new(NULL); */
/*   context = tokenizer_context(tokenizer); */
/*   g_string_printf(msg, "At line %d, column %d of input\n", context.line, context.column); */
/*   g_string_append_printf(msg, "%s\n", context.string); */
/*   g_string_append_printf(msg, "%*s^\n", context.offset, " "); */
/*   tokenizer_context_destroy(&context); */
/*   return g_string_free(msg, FALSE); */
/* } */

static int
_expect_token(
  token_type_t expected, tokenizer_t *tokenizer, GError **error,
  const char *file, int line
)
{
  token_t actual;
  GString *msg;
  tokenizer_context_t context;

  actual = tokenizer_current(tokenizer);
  if (actual.type == expected) {
    return TRUE;
  }

  _info_abort(error, file, line);

  msg = g_string_new(NULL);

  context = tokenizer_context(tokenizer);
  g_string_printf(msg, "At line %d, column %d of input\n", context.line, context.column);
  g_string_append_printf(msg, "%s\n", context.string);
  g_string_append_printf(msg, "%*s^\n", context.offset, " ");
  tokenizer_context_destroy(&context);

  g_string_append_printf(msg, "Expected %s, got %s\n", tokenizer_token_name(expected), tokenizer_token_name(actual.type));
  g_prefix_error(error, "%s", msg->str);
  g_string_free(msg, TRUE);

  return FALSE;
}
#define expect_token(expected, tokenzier, error)                \
  _expect_token(expected, tokenizer, error, __FILE__, __LINE__)

#define consume_space() \
  while (SPACE == token.type) { token = tokenizer_next(tokenizer); }

#define consume_space_string(_type) \
  while (SPACE == token.type) { token = tokenizer_next_string(tokenizer, _type); }

#define dup_token_string(token) \
  g_strndup(token.value.string.str, token.value.string.len)

/* Forward declarations */

Element *
parse_element(Document *document, tokenizer_t *tokenizer, GError **error);

void
parse_text_node(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  char *str;

  token = tokenizer_next_string(tokenizer, CHARDATA);
  if (STRING != token.type) {
    tokenizer_push(tokenizer);
    return;
  }

  if (token.value.string.whitespace_only) {
    return;
  }

  str = dup_token_string(token);
  element->append_child(doc->new_text_node(str));
  free(str);
}

typedef void (* entity_callback_t)(void *user, const char *entity_text);

static void
parse_entity(tokenizer_t *tokenizer, GError **error, entity_callback_t callback, void *user)
{
  token_t token;
  char *str;
  const char *expanded;

  token = tokenizer_next(tokenizer);
  if (! expect_token(AMP, tokenizer, error)) return;

  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return;

  str = dup_token_string(token);
  if (strcmp("lt", str) == 0) {
    expanded = "<";
  } else if (strcmp("gt", str) == 0) {
    expanded = ">";
  } else if (strcmp("amp", str) == 0) {
    expanded = "&";
  } else if (strcmp("quot", str) == 0) {
    expanded = "\"";
  } else if (strcmp("apos", str) == 0) {
    expanded = "'";
  }
  free(str);

  callback(user, expanded);

  token = tokenizer_next(tokenizer);
  if (! expect_token(SEMICOLON, tokenizer, error)) return;
}

typedef struct {
  Document *doc;
  Element *element;
} entity_text_t;

static void
add_entity_text(void *user, const char *entity_text)
{
  entity_text_t *info = (entity_text_t *)user;
  Node *text = info->doc->new_text_node(entity_text);
  info->element->append_child(text);
}

void
parse_entity_text(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  entity_text_t info;
  info.doc = doc;
  info.element = element;
  parse_entity(tokenizer, error, add_entity_text, &info);
}

static void
add_entity_attribute(void *user, const char *entity_text)
{
  GString *string = (GString *)user;
  g_string_append(string, entity_text);
}

void
parse_entity_attribute(GString *value, tokenizer_t *tokenizer, GError **error)
{
  parse_entity(tokenizer, error, add_entity_attribute, value);
}

static void
parse_char_ref(tokenizer_t *tokenizer, GError **error, entity_callback_t callback, void *user)
{
  token_t token;
  char *str;
  gunichar c;
  char *expanded;

  token = tokenizer_next(tokenizer);
  if (! expect_token(CHAR_REF, tokenizer, error)) return;

  token = tokenizer_next_string(tokenizer, INTEGER);

  str = dup_token_string(token);
  sscanf(str, "%"G_GINT32_MODIFIER"u", &c);
  free(str);

  expanded = g_ucs4_to_utf8(&c, 1, NULL, NULL, error);
  if (*error) return;

  callback(user, expanded);
  free(expanded);

  token = tokenizer_next(tokenizer);
  if (! expect_token(SEMICOLON, tokenizer, error)) return;
}

void
parse_char_ref_text(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  entity_text_t info;
  info.doc = doc;
  info.element = element;
  parse_char_ref(tokenizer, error, add_entity_text, &info);
}

void
parse_char_ref_attribute(GString *string, tokenizer_t *tokenizer, GError **error)
{
  parse_char_ref(tokenizer, error, add_entity_attribute, string);
}

void
parse_char_ref_hex(tokenizer_t *tokenizer, GError **error, entity_callback_t callback, void *user)
{
  token_t token;
  char *str;
  gunichar c;
  char *expanded;

  token = tokenizer_next(tokenizer);
  if (! expect_token(CHAR_REF_HEX, tokenizer, error)) return;

  token = tokenizer_next_string(tokenizer, HEX);

  str = dup_token_string(token);
  sscanf(str, "%06"G_GINT32_MODIFIER"X", &c);
  free(str);

  expanded = g_ucs4_to_utf8(&c, 1, NULL, NULL, error);
  if (*error) return;

  callback(user, expanded);
  free(expanded);

  token = tokenizer_next(tokenizer);
  if (! expect_token(SEMICOLON, tokenizer, error)) return;
}

void
parse_char_ref_hex_text(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  entity_text_t info;
  info.doc = doc;
  info.element = element;
  parse_char_ref_hex(tokenizer, error, add_entity_text, &info);
}

void
parse_char_ref_hex_attribute(GString *string, tokenizer_t *tokenizer, GError **error)
{
  parse_char_ref_hex(tokenizer, error, add_entity_attribute, string);
}

void
parse_end_tag(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  token_t token;

  /* TODO: Check this matches the element name */
  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return;

  token = tokenizer_next(tokenizer);
  consume_space();
  if (! expect_token(GT, tokenizer, error)) return;

  return;
}

void
parse_comment(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  Comment *comment;
  token_t token;
  char *text;

  tokenizer_next_string(tokenizer, COMMENT_TEXT);
  if (! expect_token(STRING, tokenize, error)) return;

  text = dup_token_string(token);
  comment = doc->new_comment(text);

  element->append_child(comment);

  token = tokenizer_next(tokenizer);
  if (! expect_token(COMMENT_END, tokenize, error)) return;
}

void
parse_child_element(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  Element *child;

  child = parse_element(doc, tokenizer, error);
  if (*error) return;

  element->append_child(child);

  return;
}

void
parse_attribute(tokenizer_t *tokenizer, GHashTable *attrs, GError **error)
{
  token_t token;
  char *name;
  GString *value;
  token_type_t quote_style;

  token = tokenizer_current(tokenizer);
  name = dup_token_string(token);

  token = tokenizer_next(tokenizer);
  if (! expect_token(EQ, tokenizer, error)) return;

  token = tokenizer_next(tokenizer);
  quote_style = token.type;
  if (quote_style != QUOT &&
      quote_style != APOS) {
    info_abort(error);
    return;
  }

  value = g_string_new(NULL);

  while (TRUE) {
    token = tokenizer_next_string(tokenizer, quote_style == QUOT ? ATTR_VALUE_QUOT : ATTR_VALUE_APOS);

    if (quote_style == token.type) {
      break;
    } else if (STRING == token.type) {
      char *str;
      str = dup_token_string(token);
      g_string_append(value, str);
      free(str);
    } else if (AMP == token.type) {
      tokenizer_push(tokenizer);
      parse_entity_attribute(value, tokenizer, error);
      if (*error) return;
    } else if (CHAR_REF == token.type) {
      tokenizer_push(tokenizer);
      parse_char_ref_attribute(value, tokenizer, error);
      if (*error) return;
    } else if (CHAR_REF_HEX == token.type) {
      tokenizer_push(tokenizer);
      parse_char_ref_hex_attribute(value, tokenizer, error);
      if (*error) return;
    } else {
      info_abort(error);
      return;
    }
  }

  g_hash_table_insert(attrs, name, g_string_free(value, FALSE));
}

GHashTable *
parse_attributes(tokenizer_t *tokenizer, GError **error)
{
  GHashTable *attrs;
  token_t token;

  token = tokenizer_next_string(tokenizer, NAME);
  consume_space_string(NAME);

  attrs = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

  while (STRING == token.type) {
    parse_attribute(tokenizer, attrs, error);
    if (*error) {
      return attrs;
    }

    token = tokenizer_next_string(tokenizer, NAME);
    consume_space_string(NAME);
  }

  return attrs;
}

static void
parse_element_attributes1(gpointer key_gp, gpointer value_gp, gpointer user_gp)
{
  const char *name = (const char *)key_gp;
  const char *value = (const char *)value_gp;
  Element *element = (Element *)user_gp;

  element->set_attribute(name, value);
}

void
parse_element_attributes(Document *doc, Element *element, tokenizer_t *tokenizer, GError **error)
{
  GHashTable *attrs;

  attrs = parse_attributes(tokenizer, error);
  if (*error) return;

  g_hash_table_foreach(attrs, parse_element_attributes1, element);
  g_hash_table_destroy(attrs);
}

Element *
parse_element(Document *doc, tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  Element *element;
  char *name;

  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return NULL;

  name = dup_token_string(token);
  element = doc->new_element(name);
  free(name);

  parse_element_attributes(doc, element, tokenizer, error);
  if (*error) {
    return NULL;
  }
  token = tokenizer_current(tokenizer);

  if (SLASH == token.type) {
    /* Self-closing */
    token = tokenizer_next(tokenizer);
    if (! expect_token(GT, tokenizer, error)) return NULL;
  } else if (GT == token.type) {
    /* Possible content */
    while (TRUE) {
      token = tokenizer_next_string(tokenizer, CHARDATA);

      if (CLOSE_TAG_START == token.type) {
        parse_end_tag(doc, element, tokenizer, error);
        break;
      } else if (COMMENT_START == token.type) {
        parse_comment(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else if (LT == token.type) {
        parse_child_element(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else if (STRING == token.type) {
        tokenizer_push(tokenizer);
        parse_text_node(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else if (AMP == token.type) {
        tokenizer_push(tokenizer);
        parse_entity_text(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else if (CHAR_REF == token.type) {
        tokenizer_push(tokenizer);
        parse_char_ref_text(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else if (CHAR_REF_HEX == token.type) {
        tokenizer_push(tokenizer);
        parse_char_ref_hex_text(doc, element, tokenizer, error);
        if (*error) return NULL;
      } else {
        tokenizer_push(tokenizer);
        break;
      }
    }
  } else {
    info_abort(error);
    return NULL;
  }

  return element;
}

void
parse_preamble(tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  char *name;
  GHashTable *attrs;

  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return;

  name = dup_token_string(token);
  if (strcmp("xml", name) != 0) {
    info_abort(error);
    return;
  }
  free(name);

  attrs = parse_attributes(tokenizer, error);
  if (*error) return;
  g_hash_table_destroy(attrs);

  token = tokenizer_current(tokenizer);
  if (! expect_token(PI_END, tokenizer, error)) return;
}

Document *
Document::parse(const char *input, GError **error)
{
  tokenizer_t *tokenizer;
  token_t token;
  Document *doc = new Document();
  Element *element;

  tokenizer = tokenizer_new(input);
  token = tokenizer_next_string(tokenizer, CHARDATA);

  if (PI_START == token.type) {
    parse_preamble(tokenizer, error);
    if (*error) goto error;

    token = tokenizer_next_string(tokenizer, CHARDATA);
  }

  if (STRING == token.type) {
    if (! token.value.string.whitespace_only) {
      info_abort(error);
      goto error;
    }

    token = tokenizer_next(tokenizer);
  }

  if (! expect_token(LT, tokenizer, error)) goto error;
  element = parse_element(doc, tokenizer, error);
  if (*error) goto error;
  doc->root_ = element;

  token = tokenizer_next(tokenizer);
  if (! expect_token(END, tokenizer, error)) goto error;

 cleanup:
  tokenizer_free(tokenizer);
  return doc;

 error:
  delete doc;
  doc = NULL;
  goto cleanup;
}

Element *
Document::root()
{
  return root_;
}
