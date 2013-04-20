#include "xpath-token.h"

XPathToken::XPathToken(std::string string) :
  _type(XPathTokenType::String), _prefixed_name(PrefixedName("", string))
{
}

XPathToken::XPathToken(PrefixedName prefixed_name) :
  _type(XPathTokenType::String), _prefixed_name(prefixed_name)
{
}

XPathToken::XPathToken(double number) :
  _type(XPathTokenType::Number), _prefixed_name(PrefixedName("", "")), _number(number)
{
}

XPathToken::XPathToken(XPathTokenType type) :
  _type(type), _prefixed_name(PrefixedName("", ""))
{
}

XPathToken::XPathToken(XPathTokenType type, std::string string) :
  _type(type), _prefixed_name(PrefixedName("", string))
{}

std::string
XPathToken::string() const
{
  return _prefixed_name.name();
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
  switch (_type) {
  case XPathTokenType::Slash:
  case XPathTokenType::DoubleSlash:
  case XPathTokenType::PlusSign:
  case XPathTokenType::MinusSign:
  case XPathTokenType::Pipe:
  case XPathTokenType::Equal:
  case XPathTokenType::NotEqual:
  case XPathTokenType::LessThan:
  case XPathTokenType::LessThanOrEqual:
  case XPathTokenType::GreaterThan:
  case XPathTokenType::GreaterThanOrEqual:
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
  case XPathTokenType::NodeTest:
  case XPathTokenType::Function:
  case XPathTokenType::Axis:
    return _prefixed_name == other._prefixed_name;
  case XPathTokenType::Literal:
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
  case XPathTokenType::NodeTest:
    return strm << "NodeTest";
  case XPathTokenType::Function:
    return strm << "Function";
  case XPathTokenType::Axis:
    return strm << "Axis";
  case XPathTokenType::Literal:
    return strm << "Literal";
  case XPathTokenType::Number:
    return strm << "Number";
  case XPathTokenType::CurrentNode:
    return strm << "CurrentNode";
  case XPathTokenType::ParentNode:
    return strm << "ParentNode";
  case XPathTokenType::DollarSign:
    return strm << "DollarSign";
  case XPathTokenType::PlusSign:
    return strm << "PlusSign";
  case XPathTokenType::MinusSign:
    return strm << "MinusSign";
  case XPathTokenType::Pipe:
    return strm << "Pipe";
  case XPathTokenType::Equal:
    return strm << "Equal";
  case XPathTokenType::NotEqual:
    return strm << "NotEqual";
  case XPathTokenType::LessThan:
    return strm << "LessThan";
  case XPathTokenType::LessThanOrEqual:
    return strm << "LessThanOrEqual";
  case XPathTokenType::GreaterThan:
    return strm << "GreaterThan";
  case XPathTokenType::GreaterThanOrEqual:
    return strm << "GreaterThanOrEqual";
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

  if (a.is(XPathTokenType::String) ||
      a.is(XPathTokenType::NodeTest) ||
      a.is(XPathTokenType::Function) ||
      a.is(XPathTokenType::Axis) ||
      a.is(XPathTokenType::Literal)) {
    strm << ", '" << a.string() << "'";
  } else if (a.is(XPathTokenType::Number)) {
    strm << ", " << a.number();
  }

  return strm << ")";
}
