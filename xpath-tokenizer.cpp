#include "xpath-tokenizer.h"

#include "xpath-parsing-exceptions.h"

#include <stdlib.h>
#include <vector>
#include <map>

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
        '-' == c || '=' == c ||
        '!' == c || '<' == c ||
        '>' == c || '$' == c)
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

size_t
while_not_character(std::string xpath, size_t offset, char end_char)
{
  for (; offset < xpath.size(); offset++) {
    auto c = xpath[offset];

    if (c == end_char) {
      break;
    }
  }

  return offset;
}

static std::map<char, XPathTokenType> single_char_tokens = {
  {'/',  XPathTokenType::Slash},
  {'(',  XPathTokenType::LeftParen},
  {')',  XPathTokenType::RightParen},
  {'[',  XPathTokenType::LeftBracket},
  {']',  XPathTokenType::RightBracket},
  {'@',  XPathTokenType::AtSign},
  {'$',  XPathTokenType::DollarSign},
  {'+',  XPathTokenType::PlusSign},
  {'-',  XPathTokenType::MinusSign},
  {'=',  XPathTokenType::Equal},
  {'<',  XPathTokenType::LessThan},
  {'>',  XPathTokenType::GreaterThan},
};

static std::map<char, XPathTokenType> repeated_char_tokens = {
  {':', XPathTokenType::DoubleColon},
  {'/', XPathTokenType::DoubleSlash}
};

auto quote_chars = {'\'', '"'};

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
XPathTokenizer::tokenize_literal(char quote_char)
{
  auto offset = _start;

  ++offset; // Skip over the starting quote
  auto start_of_string = offset;

  offset = while_not_character(_xpath, offset, quote_char);
  auto end_of_string = offset;

  // TODO: Test quote chars match
  ++offset; // Skip over ending quote

  _start = offset;
  auto value = _xpath.substr(start_of_string, end_of_string - start_of_string);
  return XPathToken(XPathTokenType::Literal, value);
}

XPathToken
XPathTokenizer::raw_next_token()
{
  auto c = _xpath[_start];

  auto repeated_token = repeated_char_tokens.find(c);
  if (repeated_token != repeated_char_tokens.end() && c == _xpath[_start + 1]) {
    _start += 2;
    return repeated_token->second;
  }

  if ('<' == c && '=' == _xpath[_start + 1]) {
    _start += 2;
    return XPathTokenType::LessThanOrEqual;
  }

  if ('>' == c && '=' == _xpath[_start + 1]) {
    _start += 2;
    return XPathTokenType::GreaterThanOrEqual;
  }

  auto token = single_char_tokens.find(c);
  if (token != single_char_tokens.end()) {
    _start += 1;
    return token->second;
  }

  for (auto quote_char : quote_chars) {
    if (quote_char == c) {
      return tokenize_literal(quote_char);
    }
  }

  if ('!' == c && '=' == _xpath[_start + 1]) {
    _start += 2;
    return XPathTokenType::NotEqual;
  }

  if ('.' == c && ! isdigit(_xpath[_start + 1])) {
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
  if (! has_more_tokens()) {
    throw NoMoreTokensAvailableException();
  }

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
