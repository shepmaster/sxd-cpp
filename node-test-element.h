#ifndef NODE_TEST_ELEMENT_H
#define NODE_TEST_ELEMENT_H

#include "xpath-node-test.h"
#include "prefixed-name.h"

class NodeTestElement : public XPathNodeTest {
public:
  NodeTestElement(PrefixedName name);
  void test(XPathEvaluationContext const &context, Nodeset &result) const;
  std::ostream &to_stream(std::ostream &) const;

private:
  PrefixedName _name;
};

#endif
