#include "axis-parent.h"

void
AxisParent::select_nodes(XPathEvaluationContext const & context,
                         XPathNodeTest const &node_test,
                         Nodeset &result) {
  auto parent_context = context.new_context_for(1);
  parent_context->next(context.node()->parent());

  node_test.test(*parent_context, result);
}

std::ostream &
AxisParent::to_stream(std::ostream &strm) const
{
  return strm << "parent::";
};
