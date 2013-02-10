#include "attribute.h"

Attribute::Attribute(Document *doc, const std::string name, const std::string value) :
  Node(doc, NODE_TYPE_ATTRIBUTE),
  _name(name), _value(value)
{
}

const std::string
Attribute::name() const
{
  return _name;
}

const std::string
Attribute::value() const
{
  return _value;
}

void
Attribute::output(Output &output) const
{
  output.output(" %s=\"%s\"", _name.c_str(), _value.c_str());
}

std::ostream&
Attribute::to_stream(std::ostream& os) const
{
  return os << "Attribute '" << _name << "', '" << _value<< "' <" << this << ">";
}
