#include "function-last.h"

std::string
FunctionLast::name() const
{
  return "last";
}

XPathValue
FunctionLast::evaluate(const XPathEvaluationContext &context,
                       const std::vector<XPathValue> arguments) const
{
  return XPathValue((double)context.size());
}
