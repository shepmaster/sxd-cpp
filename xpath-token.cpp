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

bool
XPathToken::precedes_node_test() const
{
  switch (_type) {
  case XPathTokenType::AtSign:
  case XPathTokenType::DoubleColon:
    return true;
  default:
    return false;
  }
}

bool
XPathToken::precedes_expression() const
{
  switch (_type) {
  case XPathTokenType::LeftParen:
  case XPathTokenType::LeftBracket:
    return true;
  default:
    return false;
  }
}

bool
XPathToken::is_operator() const
{
  // To add: Pipe, RelOps
  switch (_type) {
  case XPathTokenType::Slash:
  case XPathTokenType::DoubleSlash:
  case XPathTokenType::PlusSign:
  case XPathTokenType::MinusSign:
  case XPathTokenType::And:
  case XPathTokenType::Or:
  case XPathTokenType::Remainder:
  case XPathTokenType::Divide:
  case XPathTokenType::Multiply:
    return true;
  default:
    return false;
  }
}

bool
XPathToken::operator==(const XPathToken &other) const
{
  if (! other.is(_type)) {
    return false;
  }

  switch(_type) {
  case XPathTokenType::String:
    return _string == other._string;
  case XPathTokenType::Number:
    return _number == other._number;
  default:
    return true;
  }
}

std::ostream&
operator<<(std::ostream &strm, const XPathTokenType &a)
{
  switch (a) {
  case XPathTokenType::String:
    return strm << "String";
  case XPathTokenType::Number:
    return strm << "Number";
  case XPathTokenType::CurrentNode:
    return strm << "CurrentNode";
  case XPathTokenType::PlusSign:
    return strm << "PlusSign";
  case XPathTokenType::MinusSign:
    return strm << "MinusSign";
  case XPathTokenType::And:
    return strm << "And";
  case XPathTokenType::Or:
    return strm << "Or";
  case XPathTokenType::Remainder:
    return strm << "Remainder";
  case XPathTokenType::Divide:
    return strm << "Divide";
  case XPathTokenType::Multiply:
    return strm << "Multiply";
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
  case XPathTokenType::Slash:
    return strm << "Slash";
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
