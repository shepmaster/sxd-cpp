#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

#include <string>

class XPathValue
{
public:
  XPathValue(double value);
  XPathValue(std::string value);

  double number();
  std::string string();

private:
  double _number;
  std::string _string;
};

#endif
