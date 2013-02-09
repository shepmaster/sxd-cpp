#include "axis-descendant-or-self.h"

void
AxisDescendantOrSelf::select_nodes(Node *current_node, XPathNodeTest const & node_test, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    // Which order is correct?
    node_test.test(child, result);
    this->select_nodes(child, node_test, result);
  };
  current_node->foreach_child(child_selector);
}

std::ostream &
AxisDescendantOrSelf::to_string(std::ostream &strm) const
{
  return strm << "descendant-or-self::";
};
