#ifndef XPATH_NODE_TEST_H
#define XPATH_NODE_TEST_H

#include "node.h"
#include "nodeset.h"

class XPathNodeTest {
public:
  virtual ~XPathNodeTest() {};
  virtual void test(Node *node, Nodeset &result) const = 0;
  virtual std::ostream &to_string(std::ostream &) const = 0;
};

std::ostream &operator<<(std::ostream &, const XPathNodeTest &);

#endif
