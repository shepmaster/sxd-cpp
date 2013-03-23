#include "function-position.h"

std::string
FunctionPosition::name() const
{
  return "position";
}

XPathValue
FunctionPosition::evaluate(const XPathEvaluationContext &context,
                           const std::vector<XPathValue> arguments) const
{
  return XPathValue((double)context.position());
}
