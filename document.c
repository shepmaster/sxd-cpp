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

static void
_info_abort(const char *file, int line)
{
  fprintf(stderr, "Aborting at %s:%d\n", file, line);
  abort();
}
#define info_abort()                            \
  _info_abort(__FILE__, __LINE__)

static void
_expect_token(
  token_type_t expected, token_t actual, tokenizer_t *tokenizer,
  const char *file, int line
)
{
  if (actual.type != expected) {
    tokenizer_context_t context;

    context = tokenizer_context(tokenizer);
    fprintf(stderr, "At line %d, column %d of input\n", context.line, context.column);
    fprintf(stderr, "%s\n", context.string);
    fprintf(stderr, "%*s^\n", context.offset, " ");

    tokenizer_context_destroy(&context);

    fprintf(stderr, "Expected %s, got %s\n", tokenizer_token_name(expected), tokenizer_token_name(actual.type));
    _info_abort(file, line);
  }
}
#define expect_token(expected, actual, tokenzier)                       \
  _expect_token(expected, actual, tokenizer, __FILE__, __LINE__)

document_t *
document_parse(const char *input)
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
      element = parse_element(doc, tokenizer);
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
parse_element(document_t *doc, tokenizer_t *tokenizer)
{
  token_t token;
  element_t *element;
  char *name;

  token = tokenizer_next(tokenizer);
  expect_token(STRING, token, tokenizer);

  name = g_strndup(token.value.string.str, token.value.string.len);
  element = document_element_new(doc, name);
  free(name);

  token = tokenizer_next(tokenizer);
  consume_space();

  while (STRING == token.type) {
    token = parse_attribute(tokenizer, element);
  }

  if (SLASH == token.type) {
    /* Self-closing */
    token = tokenizer_next(tokenizer);
    expect_token(GT, token, tokenizer);
  } else if (GT == token.type) {
    /* Possible content */
    token = tokenizer_next(tokenizer);
    consume_space();
    expect_token(LT, token, tokenizer);

    token = tokenizer_next(tokenizer);
    /* if (SLASH == token.type)  */
    expect_token(SLASH, token, tokenizer);

    token = tokenizer_next(tokenizer);
    expect_token(STRING, token, tokenizer);

    token = tokenizer_next(tokenizer);
    consume_space();
    expect_token(GT, token, tokenizer);

  } else {
    info_abort();
  }

  return element;
}

token_t
parse_attribute(tokenizer_t *tokenizer, element_t *element)
{
  token_t token;
  char *name;
  char *value;
  token_type_t quote_style;

  name = dup_token_string(token);

  token = tokenizer_next(tokenizer);
  expect_token(EQ, token, tokenizer);

  token = tokenizer_next(tokenizer);
  quote_style = token.type;
  if (quote_style != QUOT &&
      quote_style != APOS) {
    info_abort();
  }

  token = tokenizer_next(tokenizer);
  expect_token(STRING, token, tokenizer);
  value = dup_token_string(token);

  element_set_attribute(element, name, value);

  token = tokenizer_next(tokenizer);
  expect_token(quote_style, token, tokenizer);

  token = tokenizer_next(tokenizer);
  consume_space();
  return token;
}

element_t *
document_root(document_t *doc)
{
  return doc->root;
}
