#ifndef FUNCTION_TRUE_H
#define FUNCTION_TRUE_H

#include "xpath-function.h"

class FunctionTrue : public XPathFunction {
  std::string name() const;
  XPathValue evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const;
};

#endif
