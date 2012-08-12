#ifndef NODE_TEST_TEXT_NODE_H
#define NODE_TEST_TEXT_NODE_H

#include "xpath-node-test.h"

class NodeTestTextNode : public XPathNodeTest {
public:
  bool include_node(Node &node);
};

#endif
