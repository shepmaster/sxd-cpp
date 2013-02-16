#include "xpath-value.h"

XPathValue::XPathValue(double value) :
  _number(value), _type(Type::Number)
{}

XPathValue::XPathValue(std::string value) :
  _string(value), _type(Type::String)
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
XPathValue::is(Type type)
{
  return type == _type;
}
