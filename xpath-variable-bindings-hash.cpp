#include "xpath-variable-bindings-hash.h"

void
XPathVariableBindingsHash::set(std::string name, XPathValue value)
{
  values.emplace(name, value);
}

bool
XPathVariableBindingsHash::has_value(std::string name) const
{
  return values.count(name) > 0;
}

XPathValue
XPathVariableBindingsHash::value_for(std::string name) const
{
  return values.at(name);
}
