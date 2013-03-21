#ifndef EXPRESSION_ROOT_NODE_H
#define EXPRESSION_ROOT_NODE_H

#include "xpath-expression.h"

class ExpressionRootNode : public XPathExpression
{
public:
  ExpressionRootNode(std::unique_ptr<XPathExpression> subexpression);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::unique_ptr<XPathExpression> _subexpression;
};

#endif
