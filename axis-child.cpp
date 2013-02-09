#include "axis-child.h"

void
AxisChild::select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    node_test.test(child, result);
  };
  current_node->foreach_child(child_selector);
}

std::ostream &
AxisChild::to_string(std::ostream &strm) const
{
  return strm << "child::";
};
