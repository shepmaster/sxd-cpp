#ifndef NODE_TEST_ELEMENT_H
#define NODE_TEST_ELEMENT_H

#include "xpath-node-test.h"

class NodeTestElement : public XPathNodeTest {
public:
  NodeTestElement(std::string name);
  void test(Node *node, Nodeset &result) const;
  std::ostream &to_stream(std::ostream &) const;

private:
  std::string _name;
};

#endif
