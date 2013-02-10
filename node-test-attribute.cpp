#include "node-test-attribute.h"
#include "attribute.h"

NodeTestAttribute::NodeTestAttribute(std::string name) :
  _name(name)
{}

void
NodeTestAttribute::test(Node *node, Nodeset &result) const {
  if (node->type() == NODE_TYPE_ATTRIBUTE) {
    Attribute *attribute = dynamic_cast<Attribute *>(node);
    if (_name == "*" || attribute->name() == _name) {
      result.add(node);
    }
  }
}

std::ostream &
NodeTestAttribute::to_stream(std::ostream &strm) const {
  return strm << "attribute(" << _name << ")";
}
