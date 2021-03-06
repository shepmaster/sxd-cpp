#include "node-test-element.h"
#include "element.h"

NodeTestElement::NodeTestElement(PrefixedName name) :
  _name(name)
{
}

void NodeTestElement::test(XPathEvaluationContext const &context, Nodeset &result) const {
  auto node = context.node();

  if (node->type() != NODE_TYPE_ELEMENT) return;

  Element *e = dynamic_cast<Element *>(node);

  if (_name.has_prefix() != e->qname().has_namespace()) return;

  if (_name.has_prefix()) {
    auto prefix_uri = context.find_namespace_for_prefix(_name.prefix());

    if (! prefix_uri) return;
    if (*prefix_uri != e->qname().namespace_uri()) return;
  }

  if (_name.name() == "*" || _name.name() == e->name()) {
    result.add(e);
  }
}

std::ostream &
NodeTestElement::to_stream(std::ostream &strm) const {
  return strm << "element(" << _name << ")";
}
