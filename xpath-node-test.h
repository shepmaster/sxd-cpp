#ifndef XPATH_NODE_TEST
#define XPATH_NODE_TEST

#include "node.h"

class XPathNodeTest {
public:
  virtual ~XPathNodeTest() {};
  virtual bool include_node(Node &node) = 0;
};

#endif
