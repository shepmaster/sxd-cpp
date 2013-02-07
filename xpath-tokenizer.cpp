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
    if ('/' == c || ':' == c) {
      break;
    }
  }

  return offset;
}

XPathToken
XPathTokenizer::next_token()
{
  auto offset = _start;
  auto current_start = _start;

  auto c = _xpath[offset];

  if (':' == c && ':' == _xpath[offset + 1]) {
    _start = offset + 2;
    return XPathToken(XPathTokenType::DoubleColon);
  } else if ('/' == c && '/' == _xpath[offset + 1]) {
    _start = offset + 2;
    return XPathToken(XPathTokenType::DoubleSlash);
  } else {
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
