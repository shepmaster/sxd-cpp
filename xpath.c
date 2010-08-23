#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node-internal.h"
#include "xpath-internal.h"

void
xpath_tokens_free(xpath_tokens_t *tokens)
{
  free(tokens->xpath);
  g_array_free(tokens->tokens, TRUE);
  free(tokens);
}

xpath_tokens_t *
xpath_tokenize(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_token_t token;
  const char *token_start = NULL;
  const char *current;

  tokens = malloc(sizeof(*tokens));
  tokens->xpath = strdup(xpath);
  tokens->tokens = g_array_new(FALSE, FALSE, sizeof(xpath_token_t));

#define ADD_TOKEN(_type, _start)		\
  {						\
    token.type = _type;				\
    token.start = _start - xpath;		\
    g_array_append_val(tokens->tokens, token);	\
  }

#define FINISH_TEXT()				\
  if (token_start) {				\
    ADD_TOKEN(TEXT, token_start);		\
    token_start = NULL;				\
  }

  for (current = xpath; *current; current++) {
    if (*current == '/') {
      FINISH_TEXT();
      ADD_TOKEN(SLASH, current);
    } else if (*current == '[') {
      FINISH_TEXT();
      ADD_TOKEN(LBRACKET, current);
    } else if (*current == ']') {
      FINISH_TEXT();
      ADD_TOKEN(RBRACKET, current);
    } else if (*current == '(') {
      FINISH_TEXT();
      ADD_TOKEN(LPAREN, current);
    } else if (*current == ')') {
      FINISH_TEXT();
      ADD_TOKEN(RPAREN, current);
    } else if (*current == '"') {
      FINISH_TEXT();
      ADD_TOKEN(QUOTE, current);
    } else if (*current == '\'') {
      FINISH_TEXT();
      ADD_TOKEN(APOS, current);
    } else {
      if (token_start == NULL) {
	token_start = current;
      }
    }
  }

  FINISH_TEXT();

#undef FINISH_TEXT
#undef ADD_TOKEN

  return tokens;
}

char *
xpath_tokens_string(xpath_tokens_t *tokens, int index)
{
  xpath_token_t *token;
  char *start_of_string;

  token = &g_array_index(tokens->tokens, xpath_token_t, index);
  start_of_string = tokens->xpath + token->start;

  if (index + 1 < tokens->tokens->len) {
    xpath_token_t *next_token;
    int len;

    next_token = &g_array_index(tokens->tokens, xpath_token_t, index + 1);
    len = next_token->start - token->start;

    return g_strndup(start_of_string, len);
  } else {
    return strdup(start_of_string);
  }
}

void
xpath_compiled_free(xpath_compiled_t *compiled)
{
  int i;

  for (i = 0; i < compiled->predicates->len; i++) {
    xpath_predicate_t *predicate;
    predicate = &g_array_index(compiled->predicates, xpath_predicate_t, i);
    free(predicate->name);
  }

  g_array_free(compiled->predicates, TRUE);
  free(compiled);
}

xpath_compiled_t *
xpath_compile(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_compiled_t *compiled;
  xpath_predicate_t predicate;
  int i;

  compiled = malloc(sizeof(*compiled));
  compiled->predicates = g_array_new(FALSE, FALSE, sizeof(xpath_predicate_t));

  tokens = xpath_tokenize(xpath);
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *token = &g_array_index(tokens->tokens, xpath_token_t, i);
    switch (token->type) {
    case TEXT:
      predicate.type = XPATH_PREDICATE_ELEMENT;
      predicate.name = xpath_tokens_string(tokens, i);
      g_array_append_val(compiled->predicates, predicate);
      break;
    default:
      break;
    }
  }

  return compiled;
}

typedef struct {
  nodeset_t *nodeset;
  xpath_predicate_type_t select;
  const char * name;
} select_xpath_children_t;

static void
xpath_select_xpath_children(gpointer node_as_gp, gpointer data_as_gp)
{
  node_t *node = node_as_gp;
  select_xpath_children_t *data = data_as_gp;
  int should_add = FALSE;

  switch (node->type) {
  case NODE_TYPE_ELEMENT:
    should_add = data->select & XPATH_PREDICATE_ELEMENT;
    if (should_add && data->name) {
      element_t *element = (element_t *)node;
      should_add = strcmp(data->name, element_name(element)) == 0;
    }
    break;
  case NODE_TYPE_TEXT_NODE:
    should_add = data->select & XPATH_PREDICATE_TEXT_NODE;
    break;
  }

  if (should_add) {
    nodeset_add(data->nodeset, node);
  }
}

nodeset_t *
xpath_select_xpath(node_t *node, xpath_predicate_type_t select, const char * const name)
{
  select_xpath_children_t data;

  data.nodeset = nodeset_new();
  data.select = select;
  data.name = name;

  g_list_foreach(node->children, xpath_select_xpath_children, &data);

  return data.nodeset;
}

nodeset_t *
xpath_apply_xpath(node_t *node, const char * const xpath)
{
  xpath_compiled_t *compiled = xpath_compile(xpath);
  xpath_predicate_t *pred = &g_array_index(compiled->predicates, xpath_predicate_t, 0);
  return xpath_select_xpath(node, pred->type, pred->name);
}
