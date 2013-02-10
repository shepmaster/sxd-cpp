#include "axis-descendant-or-self.h"
#include "axis-descendant.h"

void
AxisDescendantOrSelf::select_nodes(Node *current_node, XPathNodeTest const & node_test, Nodeset &result)
{
  node_test.test(current_node, result);
  AxisDescendant descendant;
  descendant.select_nodes(current_node, node_test, result);
}

std::ostream &
AxisDescendantOrSelf::to_stream(std::ostream &strm) const
{
  return strm << "descendant-or-self::";
};
