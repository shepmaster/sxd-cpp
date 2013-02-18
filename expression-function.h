#ifndef EXPRESSION_FUNCTION_H
#define EXPRESSION_FUNCTION_H

#include "xpath-expression.h"

class ExpressionFunction : public XPathExpression
{
public:
  ExpressionFunction(std::string name);
  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::string _name;
};

#endif
