#include "step-child.h"

StepChild::StepChild(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
StepChild::select_nodes(Node *current_node, Nodeset &step_result)
{
  auto child_selector = [&](Node *child){
    _node_test->test(child, step_result);
  };
  current_node->foreach_child(child_selector);
}
