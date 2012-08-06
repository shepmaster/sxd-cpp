#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "element.h"
#include "xpath-internal.h"
#include "xpath-functions.h"
#include "nodeset.h"
#include "predicate-value.h"
#include "predicate-function.h"
#include "predicate-equals.h"

static void
not_implemented(void)
{
  abort();
}

void
xpath_tokens_free(xpath_tokens_t *tokens)
{
  free(tokens->xpath);
  delete tokens;
}

xpath_tokens_t *
xpath_tokenize(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_token_t token;
  const char *token_start = NULL;
  const char *current;

  tokens = new xpath_tokens_t;
  tokens->xpath = strdup(xpath);

#define ADD_TOKEN(_type, _start)                \
  {                                             \
    token.type = _type;                         \
    token.start = _start - xpath;               \
    tokens->tokens.push_back(token);            \
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

  token = &tokens->tokens[index];
  start_of_string = tokens->xpath + token->start;

  if (index + 1 < tokens->tokens.size()) {
    xpath_token_t *next_token;
    int len;

    next_token = &tokens->tokens[index + 1];
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

  for (i = 0; i < compiled->steps.size(); i++) {
    xpath_step_t *step = &compiled->steps[i];
    free(step->name);
  }

  delete compiled;
}

xpath_compiled_t *
xpath_compile(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_compiled_t *compiled;
  xpath_step_t step;
  int i;

  compiled = new xpath_compiled_t;

  tokens = xpath_tokenize(xpath);
  for (i = 0; i < tokens->tokens.size(); i++) {
    xpath_token_t *token = &tokens->tokens[i];
    switch (token->type) {
    case TEXT:
      step.axis = XPATH_AXIS_CHILD;
      step.type = XPATH_NODE_TYPE_ELEMENT;
      step.name = xpath_tokens_string(tokens, i);
      compiled->steps.push_back(step);
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
  Nodeset *nodeset;
} xpath_test_step_t;

static xpath_result_t
evaluate_as_position(xpath_result_t result_value, xpath_evaluation_context_t *context)
{
  xpath_parameters_t empty;
  PredicateValue val(result_value);
  PredicateFunction fn_position(xpath_fn_position, empty);
  PredicateEquals eq(val, fn_position);

  return eq.eval(context);
}

Nodeset *
xpath_apply_predicates(Nodeset *nodeset, xpath_step_t *step)
{
  Nodeset *current_nodes;

  current_nodes = nodeset;

  for (int j = 0; j < step->predicates.size(); j++) {
    xpath_evaluation_context_t context;
    xpath_result_t result;
    Nodeset *selected_nodes;
    int i;

    XPathPredicate *predicate = step->predicates[j];
    context.nodeset = current_nodes;
    selected_nodes = new Nodeset();

    for (i = 0; i < current_nodes->count(); i++) {
      context.node = (*current_nodes)[i];
      result = predicate->eval(&context);

      if (result.type == XPATH_RESULT_TYPE_NUMERIC) {
	result = evaluate_as_position(result, &context);
      }

      if (result.type != XPATH_RESULT_TYPE_BOOLEAN) {
	not_implemented();
      }

      if (result.value.boolean == TRUE) {
	selected_nodes->add(context.node);
      }
    }

    delete current_nodes;
    current_nodes = selected_nodes;
  }

  return current_nodes;
}

static void
xpath_test_step(Node *node, xpath_test_step_t *data)
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
    data->nodeset->add(node);
  }
}

struct StepTester {
  StepTester(xpath_test_step_t *data) : data(data) {}
  void operator() (Node *node) { xpath_test_step(node, data); }
  xpath_test_step_t *data;
};

struct DownwardRecursiveTester {
  DownwardRecursiveTester(xpath_test_step_t *data) : data(data) {}
  void operator() (Node *node) {
    xpath_test_step(node, data);
    node->foreach_child(*this);
  }
  xpath_test_step_t *data;
};

struct FollowingSiblingAndAncestorTester {
  FollowingSiblingAndAncestorTester(xpath_test_step_t *data) : data(data) {}
  void operator() (Node *node) {
    node->foreach_following_sibling(DownwardRecursiveTester(data));
    node->foreach_ancestor(*this);
  }
  xpath_test_step_t *data;
};

struct PrecedingSiblingAndAncestorTester {
  PrecedingSiblingAndAncestorTester(xpath_test_step_t *data) : data(data) {}
  void operator() (Node *node) {
    node->foreach_preceding_sibling(DownwardRecursiveTester(data));
    node->foreach_ancestor(*this);
  }
  xpath_test_step_t *data;
};

Nodeset *
xpath_select_xpath_no_predicates(Node *node, xpath_step_t *step)
{
  xpath_test_step_t data;

  data.nodeset = new Nodeset();
  data.step = step;

  switch (step->axis) {
  case XPATH_AXIS_SELF:
    xpath_test_step(node, &data);
    break;
  case XPATH_AXIS_CHILD:
    node->foreach_child(StepTester(&data));
    break;
  case XPATH_AXIS_PARENT:
    xpath_test_step(node->parent(), &data);
    break;
  case XPATH_AXIS_FOLLOWING_SIBLING:
    node->foreach_following_sibling(StepTester(&data));
    break;
  case XPATH_AXIS_PRECEDING_SIBLING:
    node->foreach_preceding_sibling(StepTester(&data));
    break;
  case XPATH_AXIS_DESCENDANT:
    node->foreach_child(DownwardRecursiveTester(&data));
    break;
  case XPATH_AXIS_DESCENDANT_OR_SELF:
    xpath_test_step(node, &data);
    node->foreach_child(DownwardRecursiveTester(&data));
    break;
  case XPATH_AXIS_ANCESTOR:
    node->foreach_ancestor(StepTester(&data));
    break;
  case XPATH_AXIS_ANCESTOR_OR_SELF:
    xpath_test_step(node, &data);
    node->foreach_ancestor(StepTester(&data));
    break;
  case XPATH_AXIS_FOLLOWING:
    node->foreach_following_sibling(DownwardRecursiveTester(&data));
    node->foreach_ancestor(FollowingSiblingAndAncestorTester(&data));
    break;
  case XPATH_AXIS_PRECEDING:
    node->foreach_preceding_sibling(DownwardRecursiveTester(&data));
    node->foreach_ancestor(PrecedingSiblingAndAncestorTester(&data));
    break;
  case XPATH_AXIS_ATTRIBUTE:
  case XPATH_AXIS_NAMESPACE:
    not_implemented();
  }
  return data.nodeset;
}

Nodeset *
xpath_select_xpath_steps(Node *node, std::vector<xpath_step_t> &steps)
{
  Nodeset *result_nodes;
  int i;

  result_nodes = new Nodeset();
  result_nodes->add(node);

  for (i = 0; i < steps.size(); i++) {
    xpath_step_t *step = &steps[i];
    Nodeset *current_nodes;
    int j;

    current_nodes = new Nodeset();

    for (j = 0; j < result_nodes->count(); j++) {
      Node *current_node;
      Nodeset *selected_nodes;

      current_node = (*result_nodes)[j];
      selected_nodes = xpath_select_xpath_no_predicates(current_node, step);
      current_nodes->add_nodeset(*selected_nodes);
      delete selected_nodes;
    }

    delete result_nodes;
    result_nodes = xpath_apply_predicates(current_nodes, step);
  }

  return result_nodes;
}

Nodeset *
xpath_apply_xpath(Node *node, const char * const xpath)
{
  xpath_compiled_t *compiled;
  Nodeset *nodes;

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
    delete result->value.nodeset;
    break;
  }
}
