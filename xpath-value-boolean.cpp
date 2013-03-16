#include "xpath-value-boolean.h"

XPathValueBoolean::XPathValueBoolean(bool value) :
  _value(value)
{}

double
XPathValueBoolean::number() const {
  return 0.0;
}

std::string
XPathValueBoolean::string() const {
  return "";
}

bool
XPathValueBoolean::boolean() const {
  return _value;
}

Nodeset
XPathValueBoolean::nodeset() const {
  return Nodeset();
}

bool
XPathValueBoolean::is(XPathValue::Type type) const {
  return type == XPathValue::Type::Boolean;
}

bool
XPathValueBoolean::operator==(const XPathValueImpl &other) const {
  auto boolean = dynamic_cast<const XPathValueBoolean *>(&other);
  if (! boolean) return false;
  return _value == boolean->_value;
}

std::ostream &
XPathValueBoolean::to_stream(std::ostream &os) const
{
  return os << "XPathValueBoolean(" << (_value ? "true" : "false") << ")";
}
