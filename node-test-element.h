#ifndef NODE_TEST_ELEMENT_H
#define NODE_TEST_ELEMENT_H

#include "xpath-node-test.h"

class NodeTestElement : public XPathNodeTest {
public:
  bool include_node(Node &node);
};

#endif
