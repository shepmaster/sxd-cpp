#ifndef AXIS_DESCENDANT_H
#define AXIS_DESCENDANT_H

#include "xpath-axis.h"

/**
 * Selects all child Node%s in document order.
 */
class AxisDescendant : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
