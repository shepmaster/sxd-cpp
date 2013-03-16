#ifndef MOCK_XPATH_VALUE_IMPL_H
#define MOCK_XPATH_VALUE_IMPL_H

#include "xpath-value.h"

class MockValueImpl : public XPathValueImpl
{
public:
  MOCK_CONST_METHOD0(number, double());
  MOCK_CONST_METHOD0(string, std::string());
  MOCK_CONST_METHOD0(boolean, bool());
  MOCK_CONST_METHOD0(nodeset, Nodeset());
  MOCK_CONST_METHOD1(is, bool(XPathValue::Type type));
  MOCK_CONST_METHOD1(operator_equals, bool(const XPathValueImpl &other));
  virtual bool operator==(const XPathValueImpl &other) const {
    return operator_equals(other);
  }
};

#endif
