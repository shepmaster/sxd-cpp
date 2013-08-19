#ifndef MOCK_XPATH_NODE_TEST_H
#define MOCK_XPATH_NODE_TEST_H

#include "xpath-node-test.h"

class MockNodeTest : public XPathNodeTest {
public:
  MOCK_CONST_METHOD2(test, void(XPathEvaluationContext const &context, Nodeset &result));
};

#endif
