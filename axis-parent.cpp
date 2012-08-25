#include "axis-parent.h"

AxisParent::AxisParent(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
AxisParent::select_nodes(Node *current_node, Nodeset &result) {
  _node_test->test(current_node->parent(), result);
}
