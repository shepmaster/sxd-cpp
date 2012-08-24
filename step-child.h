#ifndef STEP_CHILD_H
#define STEP_CHILD_H

#include "xpath-step.h"
#include "xpath-node-test.h"

class StepChild : public XPathStep {
public:
  StepChild(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &step_result);
private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
