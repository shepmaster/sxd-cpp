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
  document_t * doc;

  tokenizer = tokenizer_new(input);

  doc = document_new();

  while (TRUE) {
  token_t token;
    token = tokenizer_next(tokenizer);

    if (END == token.type) break;
    if (LT == token.type) {
      element_t *element;
      element = parse_element(doc, tokenizer, error);
      if (*error) return NULL;
      doc->root = element;
    }
  }

  return doc;
}

#define consume_space() \
  while (SPACE == token.type) { token = tokenizer_next(tokenizer); }

#define dup_token_string(token) \
  g_strndup(token.value.string.str, token.value.string.len)

element_t *
parse_element1(document_t *doc, tokenizer_t *tokenizer, GError **error);

element_t *
parse_element(document_t *doc, tokenizer_t *tokenizer, GError **error)
{
  token_t token;

  token = tokenizer_next(tokenizer);
  return parse_element1(doc, tokenizer, error);
}

element_t *
parse_element1(document_t *doc, tokenizer_t *tokenizer, GError **error)
{
  token_t token;
  element_t *element;
  char *name;

  token = tokenizer_current(tokenizer);
  if (! expect_token(STRING, tokenizer, error)) return NULL;

  name = g_strndup(token.value.string.str, token.value.string.len);
  element = document_element_new(doc, name);
  free(name);

  token = tokenizer_next(tokenizer);
  consume_space();

  while (STRING == token.type) {
    parse_attribute(tokenizer, element, error);
    if (*error) {
      return NULL;
    }
    token = tokenizer_current(tokenizer);
  }

  if (SLASH == token.type) {
    /* Self-closing */
    token = tokenizer_next(tokenizer);
    if (! expect_token(GT, tokenizer, error)) return NULL;
  } else if (GT == token.type) {
    /* Possible content */
    token = tokenizer_next(tokenizer);
    consume_space();
    if (! expect_token(LT, tokenizer, error)) return NULL;

    token = tokenizer_next(tokenizer);
    if (SLASH == token.type) {
      token = tokenizer_next(tokenizer);
      if (! expect_token(STRING, tokenizer, error)) return NULL;

      token = tokenizer_next(tokenizer);
      consume_space();
      if (! expect_token(GT, tokenizer, error)) return NULL;
    } else if (STRING == token.type) {
      element_t *child;

      child = parse_element1(doc, tokenizer, error);
      if (*error) return NULL;

      node_append_child((node_t *)element, (node_t *)child);

      token = tokenizer_next(tokenizer);
      if (! expect_token(LT, tokenizer, error)) return NULL;

      token = tokenizer_next(tokenizer);
      if (! expect_token(SLASH, tokenizer, error)) return NULL;

      token = tokenizer_next(tokenizer);
      if (! expect_token(STRING, tokenizer, error)) return NULL;

      token = tokenizer_next(tokenizer);
      if (! expect_token(GT, tokenizer, error)) return NULL;
    } else {
      info_abort(error);
      return NULL;
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
  char *value;
  token_type_t quote_style;

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

  token = tokenizer_next(tokenizer);
  if (! expect_token(STRING, tokenizer, error)) return;

  value = dup_token_string(token);

  element_set_attribute(element, name, value);

  token = tokenizer_next(tokenizer);
  if (! expect_token(quote_style, tokenizer, error)) return;

  token = tokenizer_next(tokenizer);
  consume_space();
}

element_t *
document_root(document_t *doc)
{
  return doc->root;
}
