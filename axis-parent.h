#ifndef AXIS_PARENT_H
#define AXIS_PARENT_H

#include "xpath-axis.h"

/**
 * Selects the parent Node.
 */
class AxisParent : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
