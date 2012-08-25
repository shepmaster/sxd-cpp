#ifndef XPATH_STEP_H
#define XPATH_STEP_H

#include "xpath-axis.h"
#include "node.h"
#include "nodeset.h"

class XPathStep {
public:
  XPathStep(std::unique_ptr<XPathAxis> axis);

  void select_nodes(Node *current_node, Nodeset &step_result);

private:
  std::unique_ptr<XPathAxis> _axis;
};

#endif
