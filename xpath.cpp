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

class StepTester {
public:
  StepTester(const std::vector<XPathNodeTest *> &tests);

  void operator() (Node *node);
  Nodeset selected_nodes();

private:
  const std::vector<XPathNodeTest *> &_tests;
  Nodeset _nodeset;
};

StepTester::StepTester(const std::vector<XPathNodeTest *> &tests) :
  _tests(tests)
{
}

void
StepTester::operator() (Node *node)
{
  for (auto test : _tests) {
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

XPathStep::XPathStep(XPathAxis *axis) :
  axis(axis)
{
}

Nodeset
XPathStep::select_without_predicates(Node *node)
{
  StepTester test(tests);
  axis->traverse(node, std::ref(test));
  return test.selected_nodes();
}

Nodeset
XPathStep::select_with_predicates(Nodeset nodes)
{
  PotentialNodes current_nodes;

  for (int j = 0; j < nodes.count(); j++) {
    auto current_node = nodes[j];
    auto selected_nodes = select_without_predicates(current_node);
    current_nodes.add_candidates(selected_nodes);
  }

  return current_nodes.apply_predicates(_predicates);
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
    result_nodes = step.select_with_predicates(result_nodes);
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


XPath::XPath(std::string xpath)
{
}

Nodeset
XPath::select(Node *node)
{
  Nodeset selected;
  selected.add(node->first_child()->next_sibling());
  return selected;
}
