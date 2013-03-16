#include "xpath-value-string.h"

#include <stdlib.h>

XPathValueString::XPathValueString(std::string value) :
  _value(value)
{}

XPathValueString::XPathValueString(const char *value) :
  _value(value)
{}

double
XPathValueString::number() const {
  const char *start = _value.c_str();
  char *end;
  auto result = strtod(start, &end);
  if (start == end) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  return result;
}

std::string
XPathValueString::string() const {
  return _value;
}

bool
XPathValueString::boolean() const {
  return _value.length() > 0;
}

Nodeset
XPathValueString::nodeset() const {
  return Nodeset();
}

bool
XPathValueString::is(XPathValue::Type type) const {
  return type == XPathValue::Type::String;
}

bool
XPathValueString::operator==(const XPathValueImpl &other) const {
  auto string = dynamic_cast<const XPathValueString *>(&other);
  if (! string) return false;
  return _value == string->_value;
}

std::ostream &
XPathValueString::to_stream(std::ostream &os) const
{
  return os << "XPathValueString(" << _value << ")";
}
