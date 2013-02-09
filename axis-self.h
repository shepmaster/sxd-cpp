#ifndef AXIS_SELF_H
#define AXIS_SELF_H

#include "xpath-axis.h"

class AxisSelf : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  std::ostream &to_string(std::ostream &strm) const;
};

#endif