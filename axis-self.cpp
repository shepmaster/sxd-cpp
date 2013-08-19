#include "axis-self.h"

void
AxisSelf::select_nodes(XPathEvaluationContext const & context,
                       XPathNodeTest const &node_test,
                       Nodeset &result) {
  node_test.test(context.node(), result);
}

std::ostream &
AxisSelf::to_stream(std::ostream &strm) const
{
  return strm << "self::";
};
