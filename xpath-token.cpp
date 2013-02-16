#include "xpath-token.h"

XPathToken::XPathToken(std::string string) :
  _type(XPathTokenType::String), _string(string)
{
}

XPathToken::XPathToken(double number) :
  _type(XPathTokenType::Number), _number(number)
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

double
XPathToken::number() const
{
  return _number;
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
  case XPathTokenType::Number:
    return strm << "Number";
  case XPathTokenType::Apostrophe:
    return strm << "Apostrophe";
  case XPathTokenType::DoubleQuote:
    return strm << "DoubleQuote";
  case XPathTokenType::LeftParen:
    return strm << "LeftParen";
  case XPathTokenType::RightParen:
    return strm << "RightParen";
  case XPathTokenType::LeftBracket:
    return strm << "LeftBracket";
  case XPathTokenType::RightBracket:
    return strm << "RightBracket";
  case XPathTokenType::AtSign:
    return strm << "AtSign";
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
  } else if (a.is(XPathTokenType::Number)) {
    strm << ", " << a.number();
  }

  return strm << ")";
}
