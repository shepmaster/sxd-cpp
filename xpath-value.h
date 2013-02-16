#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

#include <string>

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

  double number();
  std::string string();
  bool boolean();

  bool is(Type type);

private:
  double _number;
  std::string _string;
  bool _boolean;
  Type _type;
};

#endif
