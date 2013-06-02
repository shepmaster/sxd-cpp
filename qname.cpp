#include "qname.h"

#include <iostream>

QName::QName(const std::string name) :
  _name(name)
{}

QName::QName(const std::string namespace_uri, const std::string name) :
  _name(namespace_uri, name)
{}

const bool
QName::has_namespace() const
{
  return _name.has_prefix();
}

const std::string
QName::namespace_uri() const
{
  return _name.prefix();
}

const std::string
QName::name() const
{
  return _name.name();
}

bool
QName::operator==(const QName &other) const
{
  return _name == other._name;
}

bool
QName::operator!=(const QName &other) const
{
  return _name != other._name;
}

std::ostream &
operator<<(std::ostream &os, const QName &qname)
{
  return os << qname._name;
}
