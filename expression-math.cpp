#include "expression-math.h"

ExpressionAddition::ExpressionAddition(std::shared_ptr<XPathExpression> left,
                                       std::shared_ptr<XPathExpression> right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionAddition::evaluate(const XPathEvaluationContext &context) const
{
  auto left = _left->evaluate(context);
  auto right = _right->evaluate(context);
  return XPathValue(left.number() + right.number());
}
