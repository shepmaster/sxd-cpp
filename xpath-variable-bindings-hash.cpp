#include "xpath-variable-bindings-hash.h"

void
XPathVariableBindingsHash::set(std::string name, XPathValue value)
{
  values.emplace(name, value);
}

XPathValue
XPathVariableBindingsHash::value_for(std::string name) const
{
  return values.at(name);
}
