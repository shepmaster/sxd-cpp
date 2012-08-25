#ifndef XPATH_AXIS_H
#define XPATH_AXIS_H

#include "node.h"
#include "nodeset.h"

class XPathAxis {
public:
  virtual void select_nodes(Node *current_node, Nodeset &result) = 0;
};

#endif
