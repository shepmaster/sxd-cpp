#include "prefixed-name.h"

PrefixedName::PrefixedName(const std::string namespace_prefix, const std::string name) :
  _name(name)
{}

const std::string
PrefixedName::name() const
{
  return _name;
}

bool
PrefixedName::operator==(const PrefixedName &other) const
{
  return _name == other._name;
}
