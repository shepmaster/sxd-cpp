#include "axis-self.h"

AxisSelf::AxisSelf(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
AxisSelf::select_nodes(Node *current_node, Nodeset &result) {
  _node_test->test(current_node, result);
}

std::ostream &
AxisSelf::to_string(std::ostream &strm) const
{
  return strm << "self::";
};
