#include "node-test-text.h"

void NodeTestText::test(Node *node, Nodeset &result) const {
  if (node->type() == NODE_TYPE_TEXT_NODE) {
    result.add(node);
  }
}

std::ostream &
NodeTestText::to_stream(std::ostream &strm) const {
  return strm << "text()";
}
