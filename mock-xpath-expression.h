#ifndef MOCK_XPATH_EXPRESSION_H
#define MOCK_XPATH_EXPRESSION_H

#include "xpath-expression.h"

class MockExpression : public XPathExpression {
public:
  MOCK_CONST_METHOD1(evaluate, XPathValue(const XPathEvaluationContext &context));
};

#endif
