#include "expression-negation.h"

ExpressionNegation::ExpressionNegation(std::shared_ptr<XPathExpression> expression) :
  _expression(expression)
{}

XPathValue
ExpressionNegation::evaluate(const XPathEvaluationContext &context) const
{
  auto result = _expression->evaluate(context);
  return XPathValue(-result.number());
}
