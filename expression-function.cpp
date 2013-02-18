#include "expression-function.h"

ExpressionFunction::ExpressionFunction(std::string name)
{
}

XPathValue
ExpressionFunction::evaluate(const XPathEvaluationContext &context) const
{
  return XPathValue(true);
}
