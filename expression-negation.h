#ifndef EXPRESSION_NEGATION_H
#define EXPRESSION_NEGATION_H

#include "xpath-expression.h"

class ExpressionNegation : public XPathExpression {
public:
  ExpressionNegation(std::shared_ptr<XPathExpression> expression);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _expression;
};

#endif
