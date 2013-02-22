#ifndef FUNCTION_NOT_H
#define FUNCTION_NOT_H

#include "xpath-function.h"

class FunctionNot : public XPathFunction {
public:
  std::string name() const;
  XPathValue evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const;
};

#endif
