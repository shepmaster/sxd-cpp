#ifndef NODE_TEST_NAMED_ELEMENT_H
#define NODE_TEST_NAMED_ELEMENT_H

#include <string>
#include "xpath-node-test.h"

class NodeTestNamedElement : public XPathNodeTest {
public:
  NodeTestNamedElement(std::string name);
  bool include_node(Node &node);

private:
  std::string _name;
};

#endif
