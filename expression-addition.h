#ifndef EXPRESSION_ADDITION_H
#define EXPRESSION_ADDITION_H

#include "xpath-expression.h"

class ExpressionAddition : public XPathExpression {
public:
  ExpressionAddition(std::shared_ptr<XPathExpression> left,
                     std::shared_ptr<XPathExpression> right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _left;
  std::shared_ptr<XPathExpression> _right;
};

#endif
