#include "expression-root-node.h"

#include "document.h"

ExpressionRootNode::ExpressionRootNode()
{}

XPathValue
ExpressionRootNode::evaluate(const XPathEvaluationContext &context) const
{
  Nodeset result;
  auto root_node = context.node()->document()->root();
  result.add(root_node);
  return result;
}
