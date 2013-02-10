#ifndef XPATH_AXIS_H
#define XPATH_AXIS_H

#include "to-stream.h"
#include "node.h"
#include "xpath-node-test.h"
#include "nodeset.h"

class XPathAxis : public ToStream {
public:
  virtual void select_nodes(Node *current_node, XPathNodeTest const & node_test, Nodeset &result) = 0;
};

#endif
