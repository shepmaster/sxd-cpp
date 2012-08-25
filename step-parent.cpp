#include "step-parent.h"

StepParent::StepParent(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
StepParent::select_nodes(Node *current_node, Nodeset &step_result) {
  _node_test->test(current_node->parent(), step_result);
}
