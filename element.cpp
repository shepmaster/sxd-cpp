#include <iostream>
#include <glib.h>

#include "document.h"

Element::Element(Document *doc, const QName qname) :
  Node(doc, NODE_TYPE_ELEMENT),
  _qname(qname)
{}

void
Element::output(Output &output) const
{
  output.output("<%s", name().c_str());
  for (auto pair : _attributes) {
    auto attr = pair.second;
    attr->output(output);
  }
  if (first_child()) {
    output.output(">");
    output_children(output);
    output.output("</%s>", name().c_str());
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

void
Element::foreach_attribute(foreach_fn_t fn) const
{
  for (auto pair : _attributes) {
    fn(pair.second);
  }
}

const std::string
Element::name() const
{
  return _qname.name();
}

const QName
Element::qname() const
{
  return _qname;
}

void
Element::set_namespace_prefix(const std::string prefix, const std::string namespace_uri)
{
  _namespace_prefixes[prefix] = namespace_uri;
}

const std::string *
Element::find_namespace_for_prefix(const std::string prefix)
{
  if (_namespace_prefixes.count(prefix) > 0) {
    return &_namespace_prefixes[prefix];
  }
  return nullptr;
}

std::ostream &
Element::to_stream(std::ostream& os) const
{
  return os << "Element '" << this->name() << "' <" << this << ">";
}
