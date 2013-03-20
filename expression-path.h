#ifndef EXPRESSION_PATH_H
#define EXPRESSION_PATH_H

#include "xpath-expression.h"

class ExpressionPath : public XPathExpression
{
public:
  ExpressionPath(std::vector<std::unique_ptr<XPathExpression>> steps);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::vector<std::unique_ptr<XPathExpression>> _steps;
};

#endif
