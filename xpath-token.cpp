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
XPathToken::string() const
{
  return _string;
}

bool
XPathToken::is(XPathTokenType type) const
{
  return type == _type;
}


std::ostream&
operator<<(std::ostream &strm, const XPathTokenType &a)
{
  switch (a) {
  case XPathTokenType::String:
    return strm << "String";
  case XPathTokenType::DoubleColon:
    return strm << "DoubleColon";
  case XPathTokenType::DoubleSlash:
    return strm << "DoubleSlash";
  }
}

std::ostream& operator<<(std::ostream &strm, const XPathToken &a) {
  strm << "XPathToken(" << a._type;

  if (a.is(XPathTokenType::String)) {
    strm << ", '" << a.string() << "'";
  }

  return strm << ")";
}
