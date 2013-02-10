#ifndef NODE_TEST_NODE_H
#define NODE_TEST_NODE_H

#include "xpath-node-test.h"

class NodeTestNode : public XPathNodeTest {
public:
  void test(Node *node, Nodeset &result) const;
  std::ostream &to_string(std::ostream &) const;
};

#endif
