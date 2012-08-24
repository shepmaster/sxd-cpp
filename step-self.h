#ifndef STEP_SELF_H
#define STEP_SELF_H

#include "xpath-step.h"

class StepSelf : public XPathStep {
public:
  void select_nodes(Node *current_node, Nodeset &step_result);
};

#endif
