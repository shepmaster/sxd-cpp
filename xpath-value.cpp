#include "xpath-value.h"

#include <cmath>
#include <iostream>

XPathValue::XPathValue(double value) :
  _number(value), _type(Type::Number)
{}

XPathValue::XPathValue(std::string value) :
  _string(value), _type(Type::String)
{}

XPathValue::XPathValue(const char *value) :
  _string(value), _type(Type::String)
{}

XPathValue::XPathValue(bool value) :
  _boolean(value), _type(Type::Boolean)
{}

double
XPathValue::number() const
{
  return _number;
}

std::string
XPathValue::string() const
{
  return _string;
}

bool
XPathValue::boolean() const
{
  switch (_type) {
  case Type::Number:
    return _number != 0.0 &&
      ! isnan(_number);
  case Type::String:
    return _string.length() > 0;
  case Type::Boolean:
    return _boolean;
  }
}

bool
XPathValue::is(Type type) const
{
  return type == _type;
}

bool
XPathValue::operator==(const XPathValue &other) const
{
  if (! other.is(_type)) {
    return false;
  }

  switch (_type) {
  case Type::Number:
    return _number == other.number();
  case Type::String:
    return _string == other.string();
  case Type::Boolean:
    return _boolean == other.boolean();
  }
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
  }
}

std::ostream &
operator<<(std::ostream &os, const XPathValue &value)
{
  os << "XPathValue(" << value._type;
  if (value.is(XPathValue::Type::Number)) {
    os << ", " << value.number();
  } else if (value.is(XPathValue::Type::String)) {
    os << ", '" << value.string() << "'";
  } else if (value.is(XPathValue::Type::Boolean)) {
    os << value.boolean();;
  }
  return os << ")";
}
