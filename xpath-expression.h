#ifndef XPATH_EXPRESSION_H
#define XPATH_EXPRESSION_H

#include "xpath-evaluation-context.h"
#include "xpath-value.h"

class XPathExpression : public ToStream
{
public:
  virtual XPathValue evaluate(const XPathEvaluationContext &context) const = 0;
};

#endif
