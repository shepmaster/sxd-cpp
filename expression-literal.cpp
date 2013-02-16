#include "expression-literal.h"

ExpressionLiteral::ExpressionLiteral(XPathValue value) :
  _value(value)
{
}

XPathValue
ExpressionLiteral::evaluate(const XPathEvaluationContext &context) const
{
  return _value;
}
