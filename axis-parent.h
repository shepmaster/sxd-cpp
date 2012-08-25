#ifndef AXIS_PARENT_H
#define AXIS_PARENT_H

#include "xpath-axis.h"
#include "xpath-node-test.h"

class AxisParent : public XPathAxis {
public:
  AxisParent(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &result);
private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
