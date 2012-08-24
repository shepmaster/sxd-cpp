#ifndef STEP_SELF_H
#define STEP_SELF_H

#include "xpath-step.h"
#include "xpath-node-test.h"

class StepSelf : public XPathStep {
public:
  StepSelf(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &step_result);
private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
