#ifndef XPATH_NODE_TEST_H
#define XPATH_NODE_TEST_H

#include "to-stream.h"
#include "xpath-evaluation-context.h"
#include "nodeset.h"

class XPathNodeTest : public ToStream {
public:
  virtual ~XPathNodeTest() {};
  virtual void test(XPathEvaluationContext const &context, Nodeset &result) const = 0;
};

#endif
