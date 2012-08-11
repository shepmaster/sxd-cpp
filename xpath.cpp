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

  for (auto predicate : predicates) {
    xpath_evaluation_context_t context;
    xpath_result_t result;
    Nodeset selected_nodes;

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

StepTester::StepTester(XPathStep &step) :
  _step(step)
{
}

void
StepTester::operator() (Node *node)
{
  for (auto test : _step.tests) {
    if (test->include_node(*node)) {
      _nodeset.add(node);
      return;
    }
  }
}

Nodeset
StepTester::selected_nodes()
{
  return _nodeset;
}

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

void
AxisSelf::traverse(Node *node, StepTester &test) {
  test(node);
}

void
AxisChild::traverse(Node *node, StepTester &test) {
  node->foreach_child(std::ref(test));
}

void
AxisParent::traverse(Node *node, StepTester &test) {
  test(node->parent());
}

void
AxisFollowingSibling::traverse(Node *node, StepTester &test) {
  node->foreach_following_sibling(std::ref(test));
}

void
AxisPrecedingSibling::traverse(Node *node, StepTester &test) {
  node->foreach_preceding_sibling(std::ref(test));
}

void
AxisDescendant::traverse(Node *node, StepTester &test) {
  node->foreach_child(DownwardRecursiveTester(test));
}

void
AxisDescendantOrSelf::traverse(Node *node, StepTester &test) {
  test(node);
  node->foreach_child(DownwardRecursiveTester(test));
}

void
AxisAncestor::traverse(Node *node, StepTester &test) {
  node->foreach_ancestor(std::ref(test));
}

void
AxisAncestorOrSelf::traverse(Node *node, StepTester &test) {
  test(node);
  node->foreach_ancestor(std::ref(test));
}

void
AxisFollowing::traverse(Node *node, StepTester &test) {
  node->foreach_following_sibling(DownwardRecursiveTester(test));
  node->foreach_ancestor(FollowingSiblingAndAncestorTester(test));
}

void
AxisPreceding::traverse(Node *node, StepTester &test) {
  node->foreach_preceding_sibling(DownwardRecursiveTester(test));
  node->foreach_ancestor(PrecedingSiblingAndAncestorTester(test));
}

XPathStep::XPathStep(XPathAxis *axis) :
  axis(axis)
{
}

Nodeset
xpath_select_xpath_no_predicates(Node *node, XPathStep &step)
{
  StepTester test(step);
  step.axis->traverse(node, test);
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
XPathProcessor::select_steps(std::vector<XPathStep> &steps)
{
  Nodeset result_nodes;
  result_nodes.add(_node);

  for (auto step : steps) {
    PotentialNodes current_nodes;

    for (int j = 0; j < result_nodes.count(); j++) {
      Node *current_node;
      Nodeset selected_nodes;

      current_node = result_nodes[j];
      selected_nodes = xpath_select_xpath_no_predicates(current_node, step);
      current_nodes.add_candidates(selected_nodes);
    }

    result_nodes = current_nodes.apply_predicates(step.predicates);
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
