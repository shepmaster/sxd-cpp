#include "axis-descendant-or-self.h"
#include "axis-descendant.h"

void
AxisDescendantOrSelf::select_nodes(XPathEvaluationContext const & context,
                                   XPathNodeTest const & node_test,
                                   Nodeset &result)
{
  node_test.test(context, result);
  AxisDescendant descendant;
  descendant.select_nodes(context, node_test, result);
}

std::ostream &
AxisDescendantOrSelf::to_stream(std::ostream &strm) const
{
  return strm << "descendant-or-self::";
};
