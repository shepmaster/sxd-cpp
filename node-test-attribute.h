#ifndef NODE_TEST_ATTRIBUTE_H
#define NODE_TEST_ATTRIBUTE_H

#include "xpath-node-test.h"

class NodeTestAttribute : public XPathNodeTest {
public:
  NodeTestAttribute(std::string name);
  void test(XPathEvaluationContext const &context, Nodeset &result) const;
  std::ostream &to_stream(std::ostream &) const;

private:
  std::string _name;
};

#endif
