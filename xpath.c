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

  for (i = 0; i < compiled->steps->len; i++) {
    xpath_step_t *step;
    step = &g_array_index(compiled->steps, xpath_step_t, i);
    free(step->name);
  }

  g_array_free(compiled->steps, TRUE);
  free(compiled);
}

xpath_compiled_t *
xpath_compile(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_compiled_t *compiled;
  xpath_step_t step;
  int i;

  compiled = malloc(sizeof(*compiled));
  compiled->steps = g_array_new(FALSE, FALSE, sizeof(xpath_step_t));

  tokens = xpath_tokenize(xpath);
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *token = &g_array_index(tokens->tokens, xpath_token_t, i);
    switch (token->type) {
    case TEXT:
      step.axis = XPATH_AXIS_CHILD;
      step.type = XPATH_NODE_TYPE_ELEMENT;
      step.name = xpath_tokens_string(tokens, i);
      g_array_append_val(compiled->steps, step);
      break;
    default:
      break;
    }
  }

  return compiled;
}

typedef struct {
  xpath_step_t *step;
  nodeset_t *nodeset;
} xpath_test_step_t;

static void
xpath_test_step(node_t *node, xpath_test_step_t *data)
{
  int should_add = FALSE;

  switch (node->type) {
  case NODE_TYPE_ELEMENT:
    should_add = (data->step->type & XPATH_NODE_TYPE_ELEMENT);
    if (should_add && data->step->name) {
      element_t *element = (element_t *)node;
      should_add = strcmp(data->step->name, element_name(element)) == 0;
    }
    break;
  case NODE_TYPE_TEXT_NODE:
    should_add = (data->step->type & XPATH_NODE_TYPE_TEXT_NODE);
    break;
  default:
    abort();
  }

  if (should_add) {
    nodeset_add(data->nodeset, node);
  }
}

static void
xpath_test_step_wrapper(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = data_as_gp;
  xpath_test_step(node, data);
}

static void
xpath_test_and_recur_down(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = data_as_gp;
  xpath_test_step(node, data);
  node_foreach_child(node, xpath_test_and_recur_down, data);
}

static void
xpath_test_following_siblings_and_recur_up(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = data_as_gp;
  node_foreach_following_sibling(node, xpath_test_and_recur_down, data);
  node_foreach_ancestor(node, xpath_test_following_siblings_and_recur_up, data);
}

nodeset_t *
xpath_select_xpath(node_t *node, xpath_step_t *step)
{
  xpath_test_step_t data;

  data.nodeset = nodeset_new();
  data.step = step;

  switch (step->axis) {
  case XPATH_AXIS_SELF:
    xpath_test_step(node, &data);
    break;
  case XPATH_AXIS_CHILD:
    node_foreach_child(node, xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_PARENT:
    xpath_test_step(node->parent, &data);
    break;
  case XPATH_AXIS_FOLLOWING_SIBLING:
    node_foreach_following_sibling(node, xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_PRECEDING_SIBLING:
    node_foreach_preceding_sibling(node, xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_DESCENDANT:
    node_foreach_child(node, xpath_test_and_recur_down, &data);
    break;
  case XPATH_AXIS_DESCENDANT_OR_SELF:
    xpath_test_step(node, &data);
    node_foreach_child(node, xpath_test_and_recur_down, &data);
    break;
  case XPATH_AXIS_ANCESTOR:
    node_foreach_ancestor(node, xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_ANCESTOR_OR_SELF:
    xpath_test_step(node, &data);
    node_foreach_ancestor(node, xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_FOLLOWING:
    node_foreach_following_sibling(node, xpath_test_and_recur_down, &data);
    node_foreach_ancestor(node, xpath_test_following_siblings_and_recur_up, &data);
    break;
  case XPATH_AXIS_PRECEDING:
  case XPATH_AXIS_ATTRIBUTE:
  case XPATH_AXIS_NAMESPACE:
    abort();
  }
  return data.nodeset;
}

nodeset_t *
xpath_apply_xpath(node_t *node, const char * const xpath)
{
  xpath_compiled_t *compiled = xpath_compile(xpath);
  xpath_step_t *step = &g_array_index(compiled->steps, xpath_step_t, 0);
  return xpath_select_xpath(node, step);
}
