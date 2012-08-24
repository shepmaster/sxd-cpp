#ifndef XPATH_STEP_H
#define XPATH_STEP_H

#include "node.h"
#include "nodeset.h"

class XPathStep {
public:
  virtual void select_nodes(Node *current_node, Nodeset &step_result) = 0;
};

#endif
