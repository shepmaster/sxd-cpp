#include "xpath-factory.h"

XPath
XPathFactory::compile(std::string xpath)
{
  return XPath(xpath);
}
