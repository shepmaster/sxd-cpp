#include "xpath-value.h"

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
XPathValue::number()
{
  return _number;
}

std::string
XPathValue::string()
{
  return _string;
}

bool
XPathValue::boolean()
{
  return _boolean;
}

bool
XPathValue::is(Type type)
{
  return type == _type;
}
