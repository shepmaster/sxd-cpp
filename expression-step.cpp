#include "expression-step.h"

ExpressionStep::ExpressionStep(std::shared_ptr<XPathAxis> axis,
                               std::shared_ptr<XPathNodeTest> node_test) :
  _axis(axis),
  _node_test(node_test)
{
}

XPathValue
ExpressionStep::evaluate(const XPathEvaluationContext &context) const
{
  Nodeset result;
  _axis->select_nodes(context.node(), *_node_test, result);
  return result;
}

std::ostream &
ExpressionStep::to_stream(std::ostream &strm) const
{
  return strm << "ExpressionStep (" << *_axis << *_node_test << ")";
}
