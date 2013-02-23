#ifndef EXPRESSION_PATH_H
#define EXPRESSION_PATH_H

#include "xpath-expression.h"
#include "xpath-step.h"

class ExpressionPath : public XPathExpression
{
public:
  ExpressionPath(std::vector<std::unique_ptr<XPathStep>> steps);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::vector<std::unique_ptr<XPathStep>> _steps;
};

#endif
