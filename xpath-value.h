#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

#include <string>

class XPathValue
{
public:
  enum class Type {
    Number,
    String
  };

  XPathValue(double value);
  XPathValue(std::string value);

  double number();
  std::string string();

  bool is(Type type);

private:
  double _number;
  std::string _string;
  Type _type;
};

#endif
