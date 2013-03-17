#ifndef MOCK_XPATH_VARIABLE_BINDINGS_H
#define MOCK_XPATH_VARIABLE_BINDINGS_H

#include "xpath-variable-bindings.h"

class MockVariableBindings : public XPathVariableBindings {
public:
  MOCK_CONST_METHOD1(has_value, bool(std::string name));
  MOCK_CONST_METHOD1(value_for, XPathValue(std::string name));
};

#endif
