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
  auto current_start = _start;
  auto offset = _xpath.find('/', current_start);

  if (offset != std::string::npos) {
    _start = offset + 1;
    return XPathToken(_xpath.substr(current_start, offset - current_start));
  } else {
    _start = offset;
    return XPathToken(_xpath.substr(current_start));
  }
}
