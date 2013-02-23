#ifndef XPATH_FACTORY_H
#define XPATH_FACTORY_H

#include "xpath.h"

#include <string>

class XPathFactory {
public:
  XPath compile(std::string xpath);
};

#endif
