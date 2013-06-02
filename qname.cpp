#include "qname.h"

#include <iostream>

QName::QName(const std::string name) :
  _has_namespace(false),
  _name(name)
{}

QName::QName(const std::string namespace_uri, const std::string name) :
  _has_namespace(true),
  _namespace_uri(namespace_uri),
  _name(name)
{}

const bool
QName::has_namespace() const
{
  return _has_namespace;
}

const std::string
QName::name() const
{
  return _name;
}

bool
QName::operator==(const QName &other) const
{
  if (_has_namespace != other._has_namespace) {
    return false;
  }

  if (_has_namespace) {
    return
      _namespace_uri == other._namespace_uri &&
      _name == other._name;
  } else {
    return
      _name == other._name;
  }
}

bool
QName::operator!=(const QName &other) const
{
  return ! (*this == other);
}

std::ostream &
operator<<(std::ostream &os, const QName &qname)
{
  if (qname._has_namespace) {
    return os << qname._namespace_uri << ":" << qname._name;
  } else {
    return os << qname._name;
  }
}
