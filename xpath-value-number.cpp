#include "xpath-value-number.h"
#include <cmath>

XPathValueNumber::XPathValueNumber(double value) :
  _value(value)
{}

double
XPathValueNumber::number() const {
  return _value;
}

std::string
XPathValueNumber::string() const {
  return "";
}

bool
XPathValueNumber::boolean() const {
  return _value != 0.0 &&
    ! isnan(_value);
}

Nodeset
XPathValueNumber::nodeset() const {
  return Nodeset();
}

bool XPathValueNumber::
is(XPathValue::Type type) const {
  return type == XPathValue::Type::Number;
}

bool XPathValueNumber::operator==(const XPathValueImpl &other) const {
  auto number = dynamic_cast<const XPathValueNumber *>(&other);
  if (! number) return false;
  return _value == number->_value;
}

std::ostream &
XPathValueNumber::to_stream(std::ostream &os) const
{
  return os << "XPathValueNumber(" << _value << ")";
}
