#include "axis-child.h"

AxisChild::AxisChild(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
AxisChild::select_nodes(Node *current_node, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    _node_test->test(child, result);
  };
  current_node->foreach_child(child_selector);
}

std::ostream &
AxisChild::to_string(std::ostream &strm) const
{
  return strm << "child::";
};
