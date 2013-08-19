#include "axis-descendant.h"

void
AxisDescendant::select_nodes(XPathEvaluationContext const & context,
                             XPathNodeTest const & node_test,
                             Nodeset &result)
{
  auto child_selector = [&](Node *child){
    auto child_context = context.new_context_for(1);
    child_context->next(child);

    node_test.test(*child_context, result);
    this->select_nodes(*child_context, node_test, result);
  };
  context.node()->foreach_child(child_selector);
}

std::ostream &
AxisDescendant::to_stream(std::ostream &strm) const
{
  return strm << "descendant::";
};
