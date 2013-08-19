#include "node-test-node.h"

void NodeTestNode::test(XPathEvaluationContext const &context, Nodeset &result) const {
  result.add(context.node());
}

std::ostream &
NodeTestNode::to_stream(std::ostream &strm) const {
  return strm << "node()";
}
