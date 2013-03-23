#ifndef EXPRESSION_ROOT_NODE_H
#define EXPRESSION_ROOT_NODE_H

#include "xpath-expression.h"

class ExpressionRootNode : public XPathExpression
{
public:
  ExpressionRootNode();

  XPathValue evaluate(const XPathEvaluationContext &context) const;
};

#endif
