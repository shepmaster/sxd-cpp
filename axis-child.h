#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-axis.h"

class AxisChild : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  std::ostream &to_string(std::ostream &strm) const;
};

#endif
