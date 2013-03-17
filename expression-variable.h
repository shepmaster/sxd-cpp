#ifndef EXPRESSION_VARIABLE_H
#define EXPRESSION_VARIABLE_H

#include "xpath-expression.h"

class ExpressionVariable : public XPathExpression
{
public:
  ExpressionVariable(std::string name);
  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::string _name;
};

#endif
