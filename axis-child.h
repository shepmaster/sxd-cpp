#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-axis.h"
#include "xpath-node-test.h"

class AxisChild : public XPathAxis {
public:
  AxisChild(std::unique_ptr<XPathNodeTest> &&node_test);
  void select_nodes(Node *current_node, Nodeset &result);
  std::ostream &to_string(std::ostream &strm) const;

private:
  std::unique_ptr<XPathNodeTest> _node_test;
};

#endif
