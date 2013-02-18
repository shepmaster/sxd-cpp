#include "function-false.h"

std::string
FunctionFalse::name() const
{
  return "false";
}

XPathValue
FunctionFalse::evaluate() const
{
  return XPathValue(false);
}
