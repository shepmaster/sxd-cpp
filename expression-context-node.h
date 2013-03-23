#ifndef EXPRESSION_CONTEXT_NODE_H
#define EXPRESSION_CONTEXT_NODE_H

#include "xpath-expression.h"

class ExpressionContextNode : public XPathExpression
{
public:
  ExpressionContextNode();

  XPathValue evaluate(const XPathEvaluationContext &context) const;
};

#endif
