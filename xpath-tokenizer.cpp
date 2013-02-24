#include "xpath-tokenizer.h"

#include <stdlib.h>
#include <vector>

XPathTokenizer::XPathTokenizer(std::string xpath) :
  _xpath(xpath),
  _start(0)
{
}

bool
XPathTokenizer::has_more_tokens() const
{
  if (_xpath.length() > _start) {
    return true;
  }
  return false;
}

size_t while_valid_string(std::string xpath, size_t offset)
{
  for (; offset < xpath.size(); offset++) {
    auto c = xpath[offset];

    // Would be better to test for only valid string chars
    if ('/' == c || ':' == c ||
        '(' == c || ')' == c ||
        '[' == c || ']' == c ||
        '@' == c || '\'' == c ||
        '"' == c || '+' == c ||
        '-' == c)
    {
      break;
    }
  }

  return offset;
}

bool
is_number_char(char c)
{
  return isdigit(c) || '.' == c;
}

size_t
while_valid_number(std::string xpath, size_t offset)
{
  for (; offset < xpath.size(); offset++) {
    auto c = xpath[offset];

    if (! is_number_char(c)) {
      break;
    }
  }

  return offset;
}

struct NamedOperator {
  std::string name;
  XPathTokenType type;
};

static std::vector<NamedOperator> named_operators = {
  {"and", XPathTokenType::And},
  {"or",  XPathTokenType::Or},
  {"mod", XPathTokenType::Remainder},
  {"div", XPathTokenType::Divide},
  {"*",   XPathTokenType::Multiply},
};

XPathToken
XPathTokenizer::raw_next_token()
{
  auto c = _xpath[_start];

  if (':' == c && ':' == _xpath[_start + 1]) {
    _start += 2;
    return XPathToken(XPathTokenType::DoubleColon);
  } else if ('/' == c && '/' == _xpath[_start + 1]) {
    _start += 2;
    return XPathToken(XPathTokenType::DoubleSlash);
  } else if ('/' == c) {
    _start += 1;
    return XPathTokenType::Slash;
  } else if ('(' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::LeftParen);
  } else if (')' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::RightParen);
  } else if ('[' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::LeftBracket);
  } else if (']' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::RightBracket);
  } else if ('@' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::AtSign);
  } else if ('\'' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::Apostrophe);
  } else if ('"' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::DoubleQuote);
  } else if ('+' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::PlusSign);
  } else if ('-' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::MinusSign);
  } else if ('.' == c && ! isdigit(_xpath[_start + 1])) {
    // Ugly. Should we use START / FOLLOW constructs?
    if ('.' == _xpath[_start + 1]) {
      _start += 2;
      return XPathToken(XPathTokenType::ParentNode);
    } else {
      _start += 1;
      return XPathToken(XPathTokenType::CurrentNode);
    }
  } else if (is_number_char(c)) {
    auto offset = _start;
    auto current_start = _start;

    offset = while_valid_number(_xpath, offset);

    _start = offset;
    auto value = atof(_xpath.substr(current_start, offset - current_start).c_str());

    return XPathToken(value);
  } else {
    auto offset = _start;
    auto current_start = _start;

    if (_prefer_recognition_of_operator_names) {
      for (auto named_op : named_operators) {
        auto len = named_op.name.length();
        if (0 == _xpath.compare(offset, len, named_op.name)) {
          _start += len;
          return XPathToken(named_op.type);
        }
      }
    }

    offset = while_valid_string(_xpath, offset);

    _start = offset;
    return XPathToken(_xpath.substr(current_start, offset - current_start));
  }
}

XPathToken
XPathTokenizer::next_token()
{
  auto token = raw_next_token();
  if (! (token.precedes_node_test() ||
         token.precedes_expression() ||
         token.is_operator())) {
    // See http://www.w3.org/TR/xpath/#exprlex
    _prefer_recognition_of_operator_names = true;
  } else {
    _prefer_recognition_of_operator_names = false;
  }
  return token;
}

std::ostream&
operator<<(std::ostream &strm, const XPathTokenizer &a)
{
  return strm << "XPathTokenizer ('" << a._xpath << "', position " << a._start << ")";
}
