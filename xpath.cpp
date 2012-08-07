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
#include "xpath-compiled.h"

static void
not_implemented(void)
{
  abort();
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

    XPathPredicate *predicate = step->predicates[j];
    context.nodeset = current_nodes;
    selected_nodes = new Nodeset();

    for (int i = 0; i < current_nodes->count(); i++) {
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

XPathProcessor::XPathProcessor(Node *node) :
  _node(node)
{
}

Nodeset *
XPathProcessor::apply(const char * const xpath)
{
  XPathCompiled *compiled = XPathCompiled::compile(xpath);
  Nodeset *nodes = select_steps(compiled->steps());
  delete compiled;

  return nodes;
}

Nodeset *
XPathProcessor::select_steps(std::vector<xpath_step_t> &steps)
{
  Nodeset *result_nodes;

  result_nodes = new Nodeset();
  result_nodes->add(_node);

  for (int i = 0; i < steps.size(); i++) {
    xpath_step_t *step = &steps[i];
    Nodeset *current_nodes;

    current_nodes = new Nodeset();

    for (int j = 0; j < result_nodes->count(); j++) {
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
