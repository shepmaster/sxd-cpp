#ifndef XPATH_FUNCTION_H
#define XPATH_FUNCTION_H

#include "xpath-value.h"

class XPathFunction {
public:
  virtual ~XPathFunction() {};
  virtual std::string name() const = 0;
  virtual XPathValue evaluate() const = 0;
};

#endif
