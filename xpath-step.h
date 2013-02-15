#ifndef XPATH_STEP_H
#define XPATH_STEP_H

#include "xpath-axis.h"
#include "xpath-node-test.h"
#include "node.h"
#include "nodeset.h"

#include <memory>

class XPathStep {
public:
  XPathStep(std::unique_ptr<XPathAxis> axis, std::unique_ptr<XPathNodeTest> node_test);

  void select_nodes(Node *current_node, Nodeset &step_result);

private:
  std::unique_ptr<XPathAxis> _axis;
  std::unique_ptr<XPathNodeTest> _node_test;

  friend std::ostream &operator<<(std::ostream &, const XPathStep &);
};

std::ostream &operator<<(std::ostream &, const XPathStep &);

#endif
