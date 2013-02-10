#include "axis-attribute.h"
#include "element.h"

void
AxisAttribute::select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result) {
  if (current_node->type() == NODE_TYPE_ELEMENT) {
    Element *element = dynamic_cast<Element *>(current_node);

    auto child_selector = [&](Node *attr) {
      node_test.test(attr, result);
    };
    element->foreach_attribute(child_selector);
  }
}

PrincipalNodeType
AxisAttribute::principal_node_type() {
  return PrincipalNodeType::Attribute;
};

std::ostream &
AxisAttribute::to_stream(std::ostream &strm) const
{
  return strm << "attribute::";
};
