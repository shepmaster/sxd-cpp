#include "function-false.h"

std::string
FunctionFalse::name() const
{
  return "false";
}

XPathValue
FunctionFalse::evaluate(const XPathEvaluationContext &context,
                        const std::vector<XPathValue> arguments) const
{
  return XPathValue(false);
}
