#ifndef EXPRESSION_AND_H
#define EXPRESSION_AND_H

#include "xpath-expression.h"

class ExpressionAnd : public XPathExpression {
public:
  ExpressionAnd(std::shared_ptr<XPathExpression> left,
                std::shared_ptr<XPathExpression> right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _left;
  std::shared_ptr<XPathExpression> _right;
};

#endif
