#include "expression-context-node.h"

ExpressionContextNode::ExpressionContextNode()
{}

XPathValue
ExpressionContextNode::evaluate(const XPathEvaluationContext &context) const
{
  Nodeset result;
  result.add(context.node());
  return result;
}
