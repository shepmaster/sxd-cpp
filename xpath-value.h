#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

#include <string>
#include <iosfwd>

class XPathValue
{
public:
  enum class Type {
    Number,
    String,
    Boolean
  };

  XPathValue(double value);
  XPathValue(std::string value);
  XPathValue(const char *value);
  XPathValue(bool value);

  double number() const;
  std::string string() const;
  bool boolean() const;

  bool is(Type type) const;

  bool operator==(const XPathValue &other) const;

private:
  double _number;
  std::string _string;
  bool _boolean;
  Type _type;

  friend std::ostream& operator<<(std::ostream&, const XPathValue&);
};

std::ostream& operator<<(std::ostream &, const XPathValue::Type &);
std::ostream &operator<<(std::ostream &, const XPathValue &);

#endif
