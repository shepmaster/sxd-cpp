#include "axis-parent.h"

void
AxisParent::select_nodes(XPathEvaluationContext const & context,
                         XPathNodeTest const &node_test,
                         Nodeset &result) {
  node_test.test(context.node()->parent(), result);
}

std::ostream &
AxisParent::to_stream(std::ostream &strm) const
{
  return strm << "parent::";
};
