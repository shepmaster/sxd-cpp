#ifndef STEP_PARENT_H
#define STEP_PARENT_H

#include "xpath-step.h"

class StepParent : public XPathStep {
public:
  void select_nodes(Node *current_node, Nodeset &step_result);
};

#endif
