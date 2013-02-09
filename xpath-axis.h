#ifndef XPATH_AXIS_H
#define XPATH_AXIS_H

#include "node.h"
#include "nodeset.h"

class XPathAxis {
public:
  virtual void select_nodes(Node *current_node, Nodeset &result) = 0;
  virtual std::ostream &to_string(std::ostream &) const = 0;
};

std::ostream &operator<<(std::ostream &, const XPathAxis &);

#endif
