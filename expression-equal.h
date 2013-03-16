#ifndef EXPRESSION_EQUAL_H
#define EXPRESSION_EQUAL_H

#include "xpath-expression.h"

class ExpressionEqual : public XPathExpression {
public:
  ExpressionEqual(std::shared_ptr<XPathExpression> left,
                  std::shared_ptr<XPathExpression> right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _left;
  std::shared_ptr<XPathExpression> _right;
};

#endif
