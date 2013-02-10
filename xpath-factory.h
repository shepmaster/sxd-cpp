#ifndef XPATH_FACTORY_H
#define XPATH_FACTORY_H

#include <string>
#include "xpath.h"

class XPathFactory {
public:
  XPath compile(std::string xpath);
};

struct InvalidXPathAxisException : virtual std::runtime_error
{
  InvalidXPathAxisException(std::string axis) :
    std::runtime_error(axis)
  {};
};

struct InvalidNodeTestException : virtual std::runtime_error
{
  InvalidNodeTestException(std::string name) :
    std::runtime_error(name)
  {};
};

#endif
