#include "xpath-tokenizer.h"

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
    if ('/' == c || ':' == c || '(' == c || ')' == c || '@' == c) {
      break;
    }
  }

  return offset;
}

XPathToken
XPathTokenizer::next_token()
{
  auto c = _xpath[_start];

  if (':' == c && ':' == _xpath[_start + 1]) {
    _start += 2;
    return XPathToken(XPathTokenType::DoubleColon);
  } else if ('/' == c && '/' == _xpath[_start + 1]) {
    _start += 2;
    return XPathToken(XPathTokenType::DoubleSlash);
  } else if ('(' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::LeftParen);
  } else if (')' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::RightParen);
  } else if ('@' == c) {
    _start += 1;
    return XPathToken(XPathTokenType::AtSign);
  } else {
    auto offset = _start;
    auto current_start = _start;

    if ('/' == c) {
      // Ugly. Should be it's own token?
      current_start++;
      offset++;
    }

    offset = while_valid_string(_xpath, offset);

    _start = offset;
    return XPathToken(_xpath.substr(current_start, offset - current_start));
  }
}

std::ostream&
operator<<(std::ostream &strm, const XPathTokenizer &a)
{
  return strm << "XPathTokenizer ('" << a._xpath << "', position " << a._start << ")";
}
