#include "node-test-node.h"

void NodeTestNode::test(Node *node, Nodeset &result) const {
  result.add(node);
}

std::ostream &
NodeTestNode::to_stream(std::ostream &strm) const {
  return strm << "node()";
}
