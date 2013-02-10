#ifndef AXIS_ATTRIBUTE_H
#define AXIS_ATTRIBUTE_H

#include "xpath-axis.h"

class AxisAttribute : public XPathAxis {
public:
  void select_nodes(Node *current_node, XPathNodeTest const &node_test, Nodeset &result);
  PrincipalNodeType principal_node_type();

  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
