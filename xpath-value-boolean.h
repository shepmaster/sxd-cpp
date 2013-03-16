#include "xpath-value.h"

class XPathValueBoolean : public XPathValueImpl
{
public:
  XPathValueBoolean(bool value);

  double number() const;
  std::string string() const;
  bool boolean() const;
  Nodeset nodeset() const;
  bool is(XPathValue::Type type) const;
  bool operator==(const XPathValueImpl &other) const;

  std::ostream &to_stream(std::ostream &os) const;

private:
  bool _value;
};
