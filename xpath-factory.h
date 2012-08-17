#ifndef XPATH_FACTORY_H
#define XPATH_FACTORY_H

#include <string>
#include "xpath.h"

class XPathFactory {
public:
  XPath compile(std::string xpath);
};

#endif
