#include "xpath-value-nodeset.h"

XPathValueNodeset::XPathValueNodeset(Nodeset value) :
  _value(value)
{}

double
XPathValueNodeset::number() const {
  return 0.0;
}

std::string
XPathValueNodeset::string() const {
  return "";
}

bool
XPathValueNodeset::boolean() const {
  return _value.size() > 0;
}

Nodeset
XPathValueNodeset::nodeset() const {
  return _value;
}

bool
XPathValueNodeset::is(XPathValue::Type type) const {
  return type == XPathValue::Type::Nodeset;
}

bool
XPathValueNodeset::operator==(const XPathValueImpl &other) const {
  auto nodeset = dynamic_cast<const XPathValueNodeset *>(&other);
  if (! nodeset) return false;
  return _value == nodeset->_value;
}

std::ostream &
XPathValueNodeset::to_stream(std::ostream &os) const
{
  return os << "XPathValueNodeset(" << _value << ")";
}
