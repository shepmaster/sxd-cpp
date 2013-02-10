#include <iostream>
#include <glib.h>

#include "document.h"

Element::Element(Document *doc, const std::string name) :
  Node(doc, NODE_TYPE_ELEMENT),
  _name(name)
{
}

void
Element::output(Output &output) const
{
  output.output("<%s", _name.c_str());
  for (auto pair : _attributes) {
    auto attr = pair.second;
    output.output(" %s=\"%s\"", attr->name().c_str(), attr->value().c_str());
  }
  if (first_child()) {
    output.output(">");
    output_children(output);
    output.output("</%s>", _name.c_str());
  } else {
    output.output(" />");
  }
}

Attribute *
Element::set_attribute(const std::string name, const std::string value)
{
  auto attr = document()->new_attribute(name, value);
  _attributes[name] = attr;
  return attr;
}

const std::string
Element::get_attribute(const std::string name)
{
  return _attributes[name]->value();
}

const std::string
Element::name() const
{
  return this->_name;
}

std::ostream &
Element::to_stream(std::ostream& os) const
{
  return os << "Element '" << this->name() << "' <" << this << ">";
}
