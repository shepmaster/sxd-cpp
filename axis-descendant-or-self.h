#ifndef AXIS_DESCENDANT_OR_SELF_H
#define AXIS_DESCENDANT_OR_SELF_H

#include "xpath-axis.h"
#include "xpath-node-test.h"

class AxisDescendantOrSelf : public XPathAxis {
public:
  AxisDescendantOrSelf(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &result);
private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
