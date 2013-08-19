#include "axis-child.h"

void
AxisChild::select_nodes(XPathEvaluationContext const & context,
                        XPathNodeTest const &node_test,
                        Nodeset &result)
{
  auto child_selector = [&](Node *child){
    node_test.test(child, result);
  };
  context.node()->foreach_child(child_selector);
}

std::ostream &
AxisChild::to_stream(std::ostream &strm) const
{
  return strm << "child::";
};
