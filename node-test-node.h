#ifndef NODE_TEST_NODE_H
#define NODE_TEST_NODE_H

#include "xpath-node-test.h"

class NodeTestNode : public XPathNodeTest {
public:
  void test(XPathEvaluationContext const &context, Nodeset &result) const;
  std::ostream &to_stream(std::ostream &) const;
};

#endif
