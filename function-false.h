#ifndef FUNCTION_FALSE_H
#define FUNCTION_FALSE_H

#include "xpath-function.h"

class FunctionFalse : public XPathFunction {
  std::string name() const;
  XPathValue evaluate(const XPathEvaluationContext &context,
                      const std::vector<XPathValue> arguments) const;
};

#endif
