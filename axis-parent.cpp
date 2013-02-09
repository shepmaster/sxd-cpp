#include "axis-parent.h"

void
AxisParent::select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result) {
  node_test.test(current_node->parent(), result);
}

std::ostream &
AxisParent::to_string(std::ostream &strm) const
{
  return strm << "parent::";
};
