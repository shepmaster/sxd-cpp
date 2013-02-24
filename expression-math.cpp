#include "expression-math.h"

#include "make-unique.h"

std::unique_ptr<ExpressionMath>
ExpressionMath::Addition(std::shared_ptr<XPathExpression> left,
                         std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, nullptr);
}

ExpressionMath::ExpressionMath(std::shared_ptr<XPathExpression> left,
                               std::shared_ptr<XPathExpression> right,
                               BinaryMathOp operation) :
  _left(left), _right(right)
{}

XPathValue
ExpressionMath::evaluate(const XPathEvaluationContext &context) const
{
  auto left = _left->evaluate(context);
  auto right = _right->evaluate(context);
  return XPathValue(left.number() + right.number());
}
