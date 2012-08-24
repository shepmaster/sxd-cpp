#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include <string>

class XPathToken {
public:
  XPathToken(std::string string);
  std::string string();

private:
  std::string _string;
};

#endif
