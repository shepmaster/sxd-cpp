#include "xpath-factory.h"

XPath
XPathFactory::compile(std::string xpath)
{
  std::vector<std::string> parts;
  auto offset = xpath.find('/');
  if (offset != std::string::npos) {
    parts.push_back(xpath.substr(0, offset));
    parts.push_back(xpath.substr(offset + 1, std::string::npos));
  } else {
    parts.push_back(xpath);
  }
  return XPath(parts);
}
