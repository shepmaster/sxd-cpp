#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <glib.h>

#include "element.h"
#include "xpath-internal.h"
#include "xpath-functions.h"

static void
not_implemented(void)
{
  abort();
}

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

  tokens = (xpath_tokens_t *)malloc(sizeof(*tokens));
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

  compiled = (xpath_compiled_t *)malloc(sizeof(*compiled));
  compiled->steps = g_array_new(FALSE, FALSE, sizeof(xpath_step_t));

  tokens = xpath_tokenize(xpath);
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *token = &g_array_index(tokens->tokens, xpath_token_t, i);
    switch (token->type) {
    case TEXT:
      step.axis = XPATH_AXIS_CHILD;
      step.type = XPATH_NODE_TYPE_ELEMENT;
      step.name = xpath_tokens_string(tokens, i);
      step.predicates = NULL;
      g_array_append_val(compiled->steps, step);
      break;
    default:
      break;
    }
  }
  xpath_tokens_free(tokens);

  return compiled;
}

typedef struct {
  xpath_step_t *step;
  nodeset_t *nodeset;
} xpath_test_step_t;

static xpath_result_t
eval_predicate(xpath_predicate_t *predicate, xpath_evaluation_context_t *context)
{
  xpath_result_t result;

  switch (predicate->op) {
  case XPATH_PREDICATE_OP_VALUE:
    result = predicate->info.value;
    break;
  case XPATH_PREDICATE_OP_FUNCTION:
    result = predicate->info.function.fn(context, predicate->info.function.parameters);
    break;
  case XPATH_PREDICATE_OP_EQUAL:
    {
      xpath_result_t lresult = eval_predicate(predicate->info.child.left, context);
      xpath_result_t rresult = eval_predicate(predicate->info.child.right, context);

      result.type = XPATH_RESULT_TYPE_BOOLEAN;
      if (lresult.type != rresult.type) {
	not_implemented();
      }
      switch (lresult.type) {
      case XPATH_RESULT_TYPE_BOOLEAN:
	result.value.boolean = lresult.value.boolean == rresult.value.boolean;
	break;
      case XPATH_RESULT_TYPE_NUMERIC:
	result.value.boolean = lresult.value.numeric == rresult.value.numeric;
	break;
      case XPATH_RESULT_TYPE_STRING:
	result.value.boolean = (strcmp(lresult.value.string, rresult.value.string) == 0);
	break;
      case XPATH_RESULT_TYPE_NODESET:
	not_implemented();
	break;
      }
    }
    break;
  }

  return result;
}

static xpath_result_t
evaluate_as_position(xpath_result_t result_value, xpath_evaluation_context_t *context)
{
  xpath_predicate_t pred_val;
  xpath_predicate_t pred_fn_position;
  xpath_predicate_t pred_eq;

  pred_val.op = XPATH_PREDICATE_OP_VALUE;
  pred_val.info.value = result_value;

  pred_fn_position.op = XPATH_PREDICATE_OP_FUNCTION;
  pred_fn_position.info.function.fn = xpath_fn_position;
  pred_fn_position.info.function.parameters = NULL;

  pred_eq.op = XPATH_PREDICATE_OP_EQUAL;
  pred_eq.info.child.left = &pred_val;
  pred_eq.info.child.right = &pred_fn_position;

  return eval_predicate(&pred_eq, context);
}

nodeset_t *
xpath_apply_predicates(nodeset_t *nodeset, xpath_step_t *step)
{
  nodeset_t *current_nodes;
  GList *item;

  current_nodes = nodeset;

  for (item = step->predicates; item; item = g_list_next(item)) {
    xpath_predicate_t *predicate;
    xpath_evaluation_context_t context;
    xpath_result_t result;
    nodeset_t *selected_nodes;
    int i;

    predicate = (xpath_predicate_t *)item->data;
    context.nodeset = current_nodes;
    selected_nodes = nodeset_new();

    for (i = 0; i < nodeset_count(current_nodes); i++) {
      context.node = nodeset_get(current_nodes, i);
      result = eval_predicate(predicate, &context);

      if (result.type == XPATH_RESULT_TYPE_NUMERIC) {
	result = evaluate_as_position(result, &context);
      }

      if (result.type != XPATH_RESULT_TYPE_BOOLEAN) {
	not_implemented();
      }

      if (result.value.boolean == TRUE) {
	nodeset_add(selected_nodes, context.node);
      }
    }

    nodeset_free(current_nodes);
    current_nodes = selected_nodes;
  }

  return current_nodes;
}

static void
xpath_test_step(node_t *node, xpath_test_step_t *data)
{
  int should_add = FALSE;

  switch (node->type()) {
  case NODE_TYPE_ELEMENT:
    should_add = (data->step->type & XPATH_NODE_TYPE_ELEMENT);
    if (should_add && data->step->name) {
      Element *element = (Element *)node;
      should_add = strcmp(data->step->name, element->name()) == 0;
    }
    break;
  case NODE_TYPE_TEXT_NODE:
    should_add = (data->step->type & XPATH_NODE_TYPE_TEXT_NODE);
    break;
  default:
    not_implemented();
  }

  if (should_add) {
    nodeset_add(data->nodeset, node);
  }
}

static void
xpath_test_step_wrapper(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = (xpath_test_step_t *)data_as_gp;
  xpath_test_step(node, data);
}

static void
xpath_test_and_recur_down(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = (xpath_test_step_t *)data_as_gp;
  xpath_test_step(node, data);
  node->foreach_child(xpath_test_and_recur_down, data);
}

static void
xpath_test_following_siblings_and_recur_up(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = (xpath_test_step_t *)data_as_gp;
  node->foreach_following_sibling(xpath_test_and_recur_down, data);
  node->foreach_ancestor(xpath_test_following_siblings_and_recur_up, data);
}

static void
xpath_test_preceding_siblings_and_recur_up(node_t *node, gpointer data_as_gp)
{
  xpath_test_step_t *data = (xpath_test_step_t *)data_as_gp;
  node->foreach_preceding_sibling(xpath_test_and_recur_down, data);
  node->foreach_ancestor(xpath_test_preceding_siblings_and_recur_up, data);
}

nodeset_t *
xpath_select_xpath_no_predicates(node_t *node, xpath_step_t *step)
{
  xpath_test_step_t data;

  data.nodeset = nodeset_new();
  data.step = step;

  switch (step->axis) {
  case XPATH_AXIS_SELF:
    xpath_test_step(node, &data);
    break;
  case XPATH_AXIS_CHILD:
    node->foreach_child(xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_PARENT:
    xpath_test_step(node->parent(), &data);
    break;
  case XPATH_AXIS_FOLLOWING_SIBLING:
    node->foreach_following_sibling(xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_PRECEDING_SIBLING:
    node->foreach_preceding_sibling(xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_DESCENDANT:
    node->foreach_child(xpath_test_and_recur_down, &data);
    break;
  case XPATH_AXIS_DESCENDANT_OR_SELF:
    xpath_test_step(node, &data);
    node->foreach_child(xpath_test_and_recur_down, &data);
    break;
  case XPATH_AXIS_ANCESTOR:
    node->foreach_ancestor(xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_ANCESTOR_OR_SELF:
    xpath_test_step(node, &data);
    node->foreach_ancestor(xpath_test_step_wrapper, &data);
    break;
  case XPATH_AXIS_FOLLOWING:
    node->foreach_following_sibling(xpath_test_and_recur_down, &data);
    node->foreach_ancestor(xpath_test_following_siblings_and_recur_up, &data);
    break;
  case XPATH_AXIS_PRECEDING:
    node->foreach_preceding_sibling(xpath_test_and_recur_down, &data);
    node->foreach_ancestor(xpath_test_preceding_siblings_and_recur_up, &data);
    break;
  case XPATH_AXIS_ATTRIBUTE:
  case XPATH_AXIS_NAMESPACE:
    not_implemented();
  }
  return data.nodeset;
}

nodeset_t *
xpath_select_xpath_steps(node_t *node, GArray *steps)
{
  nodeset_t *result_nodes;
  int i;

  result_nodes = nodeset_new();
  nodeset_add(result_nodes, node);

  for (i = 0; i < steps->len; i++) {
    xpath_step_t *step;
    nodeset_t *current_nodes;
    int j;

    current_nodes = nodeset_new();
    step = &g_array_index(steps, xpath_step_t, i);

    for (j = 0; j < nodeset_count(result_nodes); j++) {
      node_t *current_node;
      nodeset_t *selected_nodes;

      current_node = nodeset_get(result_nodes, j);
      selected_nodes = xpath_select_xpath_no_predicates(current_node, step);
      nodeset_add_nodeset(current_nodes, selected_nodes);
      nodeset_free(selected_nodes);
    }

    nodeset_free(result_nodes);
    result_nodes = xpath_apply_predicates(current_nodes, step);
  }

  return result_nodes;
}

nodeset_t *
xpath_apply_xpath(node_t *node, const char * const xpath)
{
  xpath_compiled_t *compiled;
  nodeset_t *nodes;

  compiled = xpath_compile(xpath);
  nodes = xpath_select_xpath_steps(node, compiled->steps);
  xpath_compiled_free(compiled);

  return nodes;
}

void
xpath_result_destroy(xpath_result_t *result)
{
  switch (result->type) {
  case XPATH_RESULT_TYPE_BOOLEAN:
    break;
  case XPATH_RESULT_TYPE_NUMERIC:
    break;
  case XPATH_RESULT_TYPE_STRING:
    free(result->value.string);
    break;
  case XPATH_RESULT_TYPE_NODESET:
    nodeset_free(result->value.nodeset);
    break;
  }
}