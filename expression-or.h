#ifndef EXPRESSION_OR_H
#define EXPRESSION_OR_H

#include "xpath-expression.h"

class ExpressionOr : public XPathExpression {
public:
  ExpressionOr(std::shared_ptr<XPathExpression> left,
               std::shared_ptr<XPathExpression> right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _left;
  std::shared_ptr<XPathExpression> _right;
};

#endif
