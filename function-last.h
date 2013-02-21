#ifndef FUNCTION_LAST_H
#define FUNCTION_LAST_H

#include "xpath-function.h"

class FunctionLast : public XPathFunction {
public:
  std::string name() const;
  XPathValue evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const;
};

#endif
