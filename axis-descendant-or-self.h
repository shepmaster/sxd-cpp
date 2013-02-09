#ifndef AXIS_DESCENDANT_OR_SELF_H
#define AXIS_DESCENDANT_OR_SELF_H

#include "xpath-axis.h"

class AxisDescendantOrSelf : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  std::ostream &to_string(std::ostream &strm) const;
};

#endif
