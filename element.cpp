#include <iostream>
#include <glib.h>

#include "document.h"

Element::Element(Document *doc, const char * const name) :
  Node(doc, NODE_TYPE_ELEMENT),
  _name(name)
{
}

void
Element::output(Output &output)
{
  output.output("<%s", _name.c_str());
  for (auto attr : _attributes) {
    output.output(" %s=\"%s\"", attr.first.c_str(), attr.second.c_str());
  }
  if (first_child()) {
    output.output(">");
    output_children(output);
    output.output("</%s>", _name.c_str());
  } else {
    output.output(" />");
  }
}

void
Element::set_attribute(const char * const name, const char * const value)
{
  _attributes[name] = value;
}

const char *
Element::get_attribute(const char * const name)
{
  return _attributes[name].c_str();
}

const char *
Element::name() const
{
  return this->_name.c_str();
}

std::ostream &
Element::to_stream(std::ostream& os) const
{
  return os << "Element '" << this->name() << "' <" << this << ">";
}
