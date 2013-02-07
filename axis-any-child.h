#ifndef AXIS_ANY_CHILD_H
#define AXIS_ANY_CHILD_H

#include "xpath-axis.h"
#include "xpath-node-test.h"

class AxisAnyChild : public XPathAxis {
public:
  AxisAnyChild(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &result);
private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
