#include "xpath-value.h"

XPathValue::XPathValue(double value) :
  _number(value)
{}

double
XPathValue::number()
{
  return _number;
}
