#include "step-parent.h"

void
StepParent::select_nodes(Node *current_node, Nodeset &step_result) {
  step_result.add(current_node->parent());
}
