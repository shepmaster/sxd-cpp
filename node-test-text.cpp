#include "node-test-text.h"

void NodeTestText::test(XPathEvaluationContext const &context, Nodeset &result) const {
  auto node = context.node();
  if (node->type() == NODE_TYPE_TEXT_NODE) {
    result.add(node);
  }
}

std::ostream &
NodeTestText::to_stream(std::ostream &strm) const {
  return strm << "text()";
}
