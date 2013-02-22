#ifndef MOCK_XPATH_FUNCTION_H
#define MOCK_XPATH_FUNCTION_H

#include "xpath-function.h"

class MockFunction : public XPathFunction {
public:
  MOCK_CONST_METHOD0(name, std::string());
  MOCK_CONST_METHOD2(evaluate, XPathValue(const XPathEvaluationContext &context,
                                          const std::vector<XPathValue> arguments));
};

#endif
