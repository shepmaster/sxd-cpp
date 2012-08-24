#include "xpath-token.h"

XPathToken::XPathToken(std::string string) :
  _type(XPathTokenType::String), _string(string)
{
}

XPathToken::XPathToken(XPathTokenType type) :
  _type(type)
{
}

std::string
XPathToken::string()
{
  return _string;
}

bool
XPathToken::is(XPathTokenType type)
{
  return type == _type;
}
