#include "function-true.h"

std::string
FunctionTrue::name() const
{
  return "true";
}

XPathValue
FunctionTrue::evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const
{
  return XPathValue(true);
}
