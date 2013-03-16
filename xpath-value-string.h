#include "xpath-value.h"

class XPathValueString : public XPathValueImpl
{
public:
  XPathValueString(std::string value);
  XPathValueString(const char *value);

  double number() const;
  std::string string() const;
  bool boolean() const;
  Nodeset nodeset() const;
  bool is(XPathValue::Type type) const;
  bool operator==(const XPathValueImpl &other) const;

  std::ostream &to_stream(std::ostream &os) const;

private:
  std::string _value;
};
