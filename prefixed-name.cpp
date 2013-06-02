#include "prefixed-name.h"

PrefixedName::PrefixedName(const std::string name) :
  _name(name)
{}

PrefixedName::PrefixedName(const std::string namespace_prefix, const std::string name) :
  _has_prefix(true), _name(name)
{}

const bool
PrefixedName::has_prefix() const
{
  return _has_prefix;
}

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
