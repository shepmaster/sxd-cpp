#include "axis-descendant.h"

void
AxisDescendant::select_nodes(Node *current_node, XPathNodeTest const & node_test, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    node_test.test(child, result);
    this->select_nodes(child, node_test, result);
  };
  current_node->foreach_child(child_selector);
}

std::ostream &
AxisDescendant::to_stream(std::ostream &strm) const
{
  return strm << "descendant::";
};
