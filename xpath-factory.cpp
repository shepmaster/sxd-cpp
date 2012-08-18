#include "xpath-factory.h"

void
find_steps(std::vector<std::string> &parts, const std::string &xpath, size_t start = 0)
{
  auto offset = xpath.find('/', start);
  if (offset != std::string::npos) {
    parts.push_back(xpath.substr(start, offset - start));
    find_steps(parts, xpath, offset + 1);
  } else {
    parts.push_back(xpath.substr(start));
  }
}

XPath
XPathFactory::compile(std::string xpath)
{
  std::vector<std::string> parts;
  find_steps(parts, xpath);
  return XPath(parts);
}
