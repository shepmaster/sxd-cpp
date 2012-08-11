#include <iostream>

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

static xpath_result_t
evaluate_as_position(xpath_result_t result_value, xpath_evaluation_context_t *context)
{
  xpath_parameters_t empty;
  PredicateValue val(result_value);
  PredicateFunction fn_position(xpath_fn_position, empty);
  PredicateEquals eq(val, fn_position);

  return eq.eval(context);
}

void
PotentialNodes::add_candidates(Nodeset &nodeset)
{
  _nodeset.add_nodeset(nodeset);
}

Nodeset
PotentialNodes::apply_predicates(std::vector<XPathPredicate *> predicates)
{
  Nodeset current_nodes;

  current_nodes = _nodeset;

  for (int j = 0; j < predicates.size(); j++) {
    xpath_evaluation_context_t context;
    xpath_result_t result;
    Nodeset selected_nodes;

    XPathPredicate *predicate = predicates[j];
    context.nodeset = &current_nodes;

    for (int i = 0; i < current_nodes.count(); i++) {
      context.node = current_nodes[i];
      result = predicate->eval(&context);

      if (result.type == XPATH_RESULT_TYPE_NUMERIC) {
        result = evaluate_as_position(result, &context);
      }

      if (result.type != XPATH_RESULT_TYPE_BOOLEAN) {
        not_implemented();
      }

      if (result.value.boolean == TRUE) {
        selected_nodes.add(context.node);
      }
    }

    current_nodes = selected_nodes;
  }

  return current_nodes;
}

bool
ElementTest::include_node(Node &node)
{
  return node.type() == NODE_TYPE_ELEMENT;
}

NamedElementTest::NamedElementTest(std::string name) :
  _name(name)
{
}

bool
NamedElementTest::include_node(Node &node)
{
  ElementTest element_test;

  if (! element_test.include_node(node)) {
    return false;
  }

  Element *element = (Element *)&node;
  return _name.compare(element->name()) == 0;
}

bool
TextTest::include_node(Node &node)
{
  return node.type() == NODE_TYPE_TEXT_NODE;
}

class StepTester {
public:
  StepTester(xpath_step_t *step) :
    _step(step)
  {
  }

  void
  operator() (Node *node)
  {
    for (int i = 0; i < _step->tests.size(); i++) {
      XPathNodeTest *test = _step->tests[i];
      if (test->include_node(*node)) {
        _nodeset.add(node);
        return;
      }
    }
  }

  Nodeset selected_nodes()
  {
    return _nodeset;
  }

private:
  xpath_step_t *_step;
  Nodeset _nodeset;
};

struct DownwardRecursiveTester {
  DownwardRecursiveTester(StepTester &test) : _test(test) {}
  void operator() (Node *node) {
    _test(node);
    node->foreach_child(*this);
  }
  StepTester &_test;
};

struct FollowingSiblingAndAncestorTester {
  FollowingSiblingAndAncestorTester(StepTester &test) : _test(test) {}
  void operator() (Node *node) {
    node->foreach_following_sibling(DownwardRecursiveTester(_test));
    node->foreach_ancestor(*this);
  }
  StepTester &_test;
};

struct PrecedingSiblingAndAncestorTester {
  PrecedingSiblingAndAncestorTester(StepTester &test) : _test(test) {}
  void operator() (Node *node) {
    node->foreach_preceding_sibling(DownwardRecursiveTester(_test));
    node->foreach_ancestor(std::ref(*this));
  }
  StepTester &_test;
};

Nodeset
xpath_select_xpath_no_predicates(Node *node, xpath_step_t *step)
{
  StepTester test(step);

  switch (step->axis) {
  case XPATH_AXIS_SELF:
    test(node);
    break;
  case XPATH_AXIS_CHILD:
    node->foreach_child(std::ref(test));
    break;
  case XPATH_AXIS_PARENT:
    test(node->parent());
    break;
  case XPATH_AXIS_FOLLOWING_SIBLING:
    node->foreach_following_sibling(std::ref(test));
    break;
  case XPATH_AXIS_PRECEDING_SIBLING:
    node->foreach_preceding_sibling(std::ref(test));
    break;
  case XPATH_AXIS_DESCENDANT:
    node->foreach_child(DownwardRecursiveTester(test));
    break;
  case XPATH_AXIS_DESCENDANT_OR_SELF:
    test(node);
    node->foreach_child(DownwardRecursiveTester(test));
    break;
  case XPATH_AXIS_ANCESTOR:
    node->foreach_ancestor(std::ref(test));
    break;
  case XPATH_AXIS_ANCESTOR_OR_SELF:
    test(node);
    node->foreach_ancestor(std::ref(test));
    break;
  case XPATH_AXIS_FOLLOWING:
    node->foreach_following_sibling(DownwardRecursiveTester(test));
    node->foreach_ancestor(FollowingSiblingAndAncestorTester(test));
    break;
  case XPATH_AXIS_PRECEDING:
    node->foreach_preceding_sibling(DownwardRecursiveTester(test));
    node->foreach_ancestor(PrecedingSiblingAndAncestorTester(test));
    break;
  case XPATH_AXIS_ATTRIBUTE:
  case XPATH_AXIS_NAMESPACE:
    not_implemented();
  }

  return test.selected_nodes();
}

XPathProcessor::XPathProcessor(Node *node) :
  _node(node)
{
}

Nodeset
XPathProcessor::apply(const char * const xpath)
{
  XPathCompiled *compiled = XPathCompiled::compile(xpath);
  Nodeset nodes = select_steps(compiled->steps());
  delete compiled;

  return nodes;
}

Nodeset
XPathProcessor::select_steps(std::vector<xpath_step_t> &steps)
{
  Nodeset result_nodes;
  result_nodes.add(_node);

  for (int i = 0; i < steps.size(); i++) {
    xpath_step_t *step = &steps[i];
    PotentialNodes current_nodes;

    for (int j = 0; j < result_nodes.count(); j++) {
      Node *current_node;
      Nodeset selected_nodes;

      current_node = result_nodes[j];
      selected_nodes = xpath_select_xpath_no_predicates(current_node, step);
      current_nodes.add_candidates(selected_nodes);
    }

    result_nodes = current_nodes.apply_predicates(step->predicates);
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
