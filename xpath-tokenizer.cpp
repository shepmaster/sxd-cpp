#include "xpath-tokenizer.h"

XPathTokenizer::XPathTokenizer(std::string xpath) :
  _xpath(xpath),
  _start(0)
{
}

bool
XPathTokenizer::has_more_tokens()
{
  if (_xpath.length() > _start) {
    return true;
  }
  return false;
}

XPathToken
XPathTokenizer::next_token()
{
  auto offset = _start;
  auto current_start = _start;

  for (; offset < _xpath.size(); offset++) {
    auto c = _xpath[offset];

    if (':' == c && ':' == _xpath[offset + 1]) {
      if (offset != current_start) {
        _start = offset;
        return XPathToken(_xpath.substr(current_start, offset - current_start));
      } else {
        _start = offset + 2;
        return XPathToken(XPathTokenType::DoubleColon);
      }
    } else if ('/' == c) {
      _start = offset + 1;
      return XPathToken(_xpath.substr(current_start, offset - current_start));
    }
  }

  _start = offset;
  return XPathToken(_xpath.substr(current_start));
}
