#include "step-self.h"

StepSelf::StepSelf(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
StepSelf::select_nodes(Node *current_node, Nodeset &step_result) {
  _node_test->test(current_node, step_result);
}
