#include "axis-self.h"

void
AxisSelf::select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result) {
  node_test.test(current_node, result);
}

std::ostream &
AxisSelf::to_string(std::ostream &strm) const
{
  return strm << "self::";
};
