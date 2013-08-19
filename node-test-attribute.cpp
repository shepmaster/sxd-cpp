#include "node-test-attribute.h"
#include "attribute.h"

NodeTestAttribute::NodeTestAttribute(std::string name) :
  _name(name)
{}

void
NodeTestAttribute::test(XPathEvaluationContext const &context, Nodeset &result) const {
  auto node = context.node();

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
