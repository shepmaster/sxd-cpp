#ifndef XPATH_VARIABLE_BINDINGS_HASH_H
#define XPATH_VARIABLE_BINDINGS_HASH_H

#include "xpath-variable-bindings.h"

#include <map>

class XPathVariableBindingsHash : public XPathVariableBindings {
public:
  void set(std::string name, XPathValue value);

  bool has_value(std::string name) const;
  XPathValue value_for(std::string name) const;

private:
  std::map<std::string, XPathValue> values;
};

#endif
