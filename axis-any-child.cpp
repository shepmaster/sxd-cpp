#include "axis-any-child.h"

AxisAnyChild::AxisAnyChild(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
AxisAnyChild::select_nodes(Node *current_node, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    // Which order is correct?
    _node_test->test(child, result);
    this->select_nodes(child, result);
  };
  current_node->foreach_child(child_selector);
}
