#include "xpath-value.h"

#include "xpath-value-boolean.h"
#include "xpath-value-nodeset.h"
#include "xpath-value-number.h"
#include "xpath-value-string.h"

#include "make-unique.h"
#include <iostream>

XPathValue::XPathValue(double value) :
  _impl(make_unique<XPathValueNumber>(value))
{}

XPathValue::XPathValue(std::string value) :
  _impl(make_unique<XPathValueString>(value))
{}

XPathValue::XPathValue(const char *value) :
  _impl(make_unique<XPathValueString>(value))
{}

XPathValue::XPathValue(bool value) :
  _impl(make_unique<XPathValueBoolean>(value))
{}

XPathValue::XPathValue(Nodeset value) :
  _impl(make_unique<XPathValueNodeset>(value))
{}

double
XPathValue::number() const
{
  return _impl->number();
}

std::string
XPathValue::string() const
{
  return _impl->string();
}

bool
XPathValue::boolean() const
{
  return _impl->boolean();
}

Nodeset
XPathValue::nodeset() const
{
  return _impl->nodeset();
}

bool
XPathValue::is(Type type) const
{
  return _impl->is(type);
}

bool
XPathValue::operator==(const XPathValue &other) const
{
  return *_impl == *other._impl;
}

std::ostream &
operator<<(std::ostream &os, const XPathValue &value)
{
  return os << *value._impl;
}

std::ostream&
operator<<(std::ostream &os, const XPathValue::Type &type)
{
  switch (type) {
  case XPathValue::Type::Number:
    return os << "Number";
  case XPathValue::Type::String:
    return os << "String";
  case XPathValue::Type::Boolean:
    return os << "Boolean";
  case XPathValue::Type::Nodeset:
    return os << "Nodeset";
  }
}
