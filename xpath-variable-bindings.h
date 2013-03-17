#ifndef XPATH_VARIABLE_BINDINGS_H
#define XPATH_VARIABLE_BINDINGS_H

#include "xpath-value.h"

class XPathVariableBindings {
public:
  virtual ~XPathVariableBindings() {}
  virtual XPathValue value_for(std::string name) const = 0;
};

#endif
