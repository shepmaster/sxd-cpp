#include "node-test-element.h"
#include "element.h"

NodeTestElement::NodeTestElement(std::string name) :
  _name(name)
{
}

void NodeTestElement::test(Node *node, Nodeset &result) const {
  if (node->type() == NODE_TYPE_ELEMENT) {
    Element *e = dynamic_cast<Element *>(node);
    if (_name == "*" || _name == e->name()) {
      result.add(e);
    }
  }
}

std::ostream &
NodeTestElement::to_string(std::ostream &strm) const {
  return strm << _name;
}
