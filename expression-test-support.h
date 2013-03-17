#ifndef EXPRESSION_TEST_SUPPORT_H
#define EXPRESSION_TEST_SUPPORT_H

#include "make-unique.h"

class ExpressionTestSupport {
public:
  Node *node;
  Nodeset nodes;
  XPathFunctionLibrary functions;

  XPathEvaluationContext &context() {
    if (! _context) {
      _context = make_unique<XPathEvaluationContext>(node, nodes, functions);
    }

    return *_context;
  }

private:
  std::unique_ptr<XPathEvaluationContext> _context;
};

#endif
