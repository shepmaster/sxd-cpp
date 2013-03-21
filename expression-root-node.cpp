#include "expression-root-node.h"

#include "document.h"

ExpressionRootNode::ExpressionRootNode(std::unique_ptr<XPathExpression> subexpression) :
  _subexpression(move(subexpression))
{}

XPathValue
ExpressionRootNode::evaluate(const XPathEvaluationContext &context) const
{
  auto root_node = context.node()->document()->root();
  auto sub_context = context.new_context_for(1);
  sub_context->next(root_node);
  return _subexpression->evaluate(*sub_context);
}
