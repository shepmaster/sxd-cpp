#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document-internal.h"
#include "node-internal.h"
#include "element-internal.h"
#include "intern.h"

struct documentS {
  element_t *root;
  intern_t *dict;
  unsigned int managed_node_count;
};

void
document_free(document_t *doc)
{
  if (! doc) return;

  intern_free(doc->dict);
  free(doc);
}

document_t *
document_new(void)
{
  document_t *doc;

  doc = calloc(1, sizeof(*doc));
  doc->dict = intern_new();

  return doc;
}

element_t *
document_element_new(document_t *doc, const char * const name)
{
  element_t *e = element_new(doc, name);
  if (e) {
    doc->managed_node_count++;
  }
  return e;
}

text_node_t *
document_text_node_new(document_t *doc, const char * const text)
{
  return text_node_new(doc, text);
}

const char *
document_intern(document_t *doc, const char * const string)
{
  return intern_intern(doc->dict, string);
}

unsigned int
document_managed_node_count(document_t *doc)
{
  return doc->managed_node_count;
}

void
document_stop_managing_node(document_t *doc, node_t *node)
{
  doc->managed_node_count--;
}

void
document_manage_node(document_t *doc, node_t *node)
{
  node_change_document(node, doc);
  doc->managed_node_count++;
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

document_t *
document_parse(const char *input, GError **error)
{
  tokenizer_t *tokenizer;
  token_t token;
  document_t *doc;
  element_t *element;

  doc = document_new();

  tokenizer = tokenizer_new(input);
  token = tokenizer_next_string(tokenizer, CHARDATA);

  if (PI_START == token.type) {
    parse_preamble(tokenizer, error);
    if (*error) return NULL;

    token = tokenizer_next_string(tokenizer, CHARDATA);
  }

  if (STRING == token.type) {
    if (! token.value.string.whitespace_only) {
      info_abort(error);
      return NULL;
    }

    token = tokenizer_next(tokenizer);
  }

  if (! expect_token(LT, tokenizer, error)) return NULL;
  element = parse_element(doc, tokenizer, error);
  if (*error) return NULL;
  doc->root = element;

  token = tokenizer_next(tokenizer);
  if (! expect_token(END, tokenizer, error)) return NULL;

  return doc;
}

#define consume_space() \
  while (SPACE == token.type) { token = tokenizer_next(tokenizer); }

#define consume_space_string(_type) \
  while (SPACE == token.type) { token = tokenizer_next_string(tokenizer, _type); }

#define dup_token_string(token) \
  g_strndup(token.value.string.str, token.value.string.len)

void
parse_preamble(tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  char *name;

  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return;

  name = dup_token_string(token);
  if (strcmp("xml", name) != 0) {
    info_abort(error);
    return;
  }
  free(name);

  token = tokenizer_next(tokenizer);
  if (! expect_token(PI_END, tokenizer, error)) return;
}

void
parse_text_node(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
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
  node_append_child((node_t *)element, (node_t *)document_text_node_new(doc, str));
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
  document_t *doc;
  element_t *element;
} entity_text_t;

static void
add_entity_text(void *user, const char *entity_text)
{
  entity_text_t *info = user;
  node_t *text;

  text = (node_t *)document_text_node_new(info->doc, entity_text);
  node_append_child((node_t *)info->element, text);
}

void
parse_entity_text(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
{
  entity_text_t info;
  info.doc = doc;
  info.element = element;
  parse_entity(tokenizer, error, add_entity_text, &info);
}

static void
add_entity_attribute(void *user, const char *entity_text)
{
  GString *string = user;
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
parse_char_ref_text(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
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
parse_char_ref_hex_text(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
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
parse_end_tag(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
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
parse_child_element(document_t *doc, element_t *element, tokenizer_t *tokenizer, GError **error)
{
  element_t *child;

  child = parse_element(doc, tokenizer, error);
  if (*error) return;

  node_append_child((node_t *)element, (node_t *)child);

  return;
}

element_t *
parse_element(document_t *doc, tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  element_t *element;
  char *name;

  token = tokenizer_next_string(tokenizer, NAME);
  if (! expect_token(STRING, tokenizer, error)) return NULL;

  name = dup_token_string(token);
  element = document_element_new(doc, name);
  free(name);

  token = tokenizer_next_string(tokenizer, NAME);
  consume_space_string(NAME);

  while (STRING == token.type) {
    parse_attribute(tokenizer, element, error);
    if (*error) {
      return NULL;
    }

    token = tokenizer_next_string(tokenizer, NAME);
    consume_space_string(NAME);
  }

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
parse_attribute(tokenizer_t *tokenizer, element_t *element, GError **error)
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

  element_set_attribute(element, name, value->str);
  g_string_free(value, TRUE);
}

element_t *
document_root(document_t *doc)
{
  return doc->root;
}
