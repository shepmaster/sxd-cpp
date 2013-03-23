#ifndef FUNCTION_POSITION_H
#define FUNCTION_POSITION_H

#include "xpath-function.h"

class FunctionPosition : public XPathFunction {
public:
  std::string name() const;
  XPathValue evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const;
};

#endif
