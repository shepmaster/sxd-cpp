#include "prefixed-name.h"

#include <iostream>

PrefixedName::PrefixedName(const std::string name) :
  _has_prefix(false),
  _name(name)
{}

PrefixedName::PrefixedName(const std::string prefix, const std::string name) :
  _has_prefix(true),
  _prefix(prefix),
  _name(name)
{}

const bool
PrefixedName::has_prefix() const
{
  return _has_prefix;
}

const std::string
PrefixedName::prefix() const
{
  return _prefix;
}

const std::string
PrefixedName::name() const
{
  return _name;
}

bool
PrefixedName::operator==(const PrefixedName &other) const
{
  if (_has_prefix != other._has_prefix) {
    return false;
  }

  if (_has_prefix) {
    return
      _prefix == other._prefix &&
      _name == other._name;
  } else {
    return
      _name == other._name;
  }
}

bool
PrefixedName::operator!=(const PrefixedName &other) const
{
  return ! (*this == other);
}

std::ostream &
operator<<(std::ostream &os, const PrefixedName &obj)
{
  if (obj._has_prefix) {
    return os << obj._prefix << ":" << obj._name;
  } else {
    return os << obj._name;
  }
}
