#ifndef STEP_CHILD_H
#define STEP_CHILD_H

#include "xpath-step.h"

class StepChild : public XPathStep {
public:
  StepChild(std::string name);
  void select_nodes(Node *current_node, Nodeset &step_result);
private:
  std::string _name;
};

#endif
