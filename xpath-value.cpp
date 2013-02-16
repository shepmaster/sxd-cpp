#include "xpath-value.h"

XPathValue::XPathValue(double value) :
  _number(value)
{}

XPathValue::XPathValue(std::string value) :
  _string(value)
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
