#include "function-not.h"

std::string
FunctionNot::name() const
{
  return "not";
}

XPathValue
FunctionNot::evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const
{
  return XPathValue(! arguments[0].boolean());
}
