#ifndef NODE_TEST_ELEMENT_H
#define NODE_TEST_ELEMENT_H

#include "xpath-node-test.h"

class NodeTestElement : public XPathNodeTest {
public:
  NodeTestElement(std::string name);
  void test(Node *node, Nodeset &result);

private:
  std::string _name;
};

#endif
