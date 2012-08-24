#include "xpath-token.h"

XPathToken::XPathToken(std::string string) :
  _string(string)
{
}

std::string
XPathToken::string()
{
  return _string;
}
