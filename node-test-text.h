#ifndef NODE_TEST_TEXT_H
#define NODE_TEST_TEXT_H

#include "xpath-node-test.h"

class NodeTestText : public XPathNodeTest {
public:
  void test(Node *node, Nodeset &result) const;
  std::ostream &to_stream(std::ostream &) const;
};

#endif
