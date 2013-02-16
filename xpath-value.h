#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

class XPathValue
{
public:
  XPathValue(double value);
  double number();

private:
  double _number;
};

#endif
