#ifndef XPATH_NODE_TEST_H
#define XPATH_NODE_TEST_H

#include "to-stream.h"
#include "node.h"
#include "nodeset.h"

class XPathNodeTest : public ToStream {
public:
  virtual ~XPathNodeTest() {};
  virtual void test(Node *node, Nodeset &result) const = 0;
};

#endif
