#ifndef EXPRESSION_LITERAL_H
#define EXPRESSION_LITERAL_H

#include "xpath-expression.h"

class ExpressionLiteral : public XPathExpression
{
public:
  ExpressionLiteral(XPathValue value);
  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  XPathValue _value;
};

#endif
