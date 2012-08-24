#include "step-self.h"

void
StepSelf::select_nodes(Node *current_node, Nodeset &step_result) {
  step_result.add(current_node);
}
