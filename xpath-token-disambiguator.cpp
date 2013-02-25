#include "xpath-token-disambiguator.h"

XPathTokenDisambiguator::XPathTokenDisambiguator(XPathRawTokenSource &token_source) :
  _token_source(token_source)
{}

bool
XPathTokenDisambiguator::has_more_tokens() const
{
  return !_buffer.empty() || _token_source.has_more_tokens();
}

XPathToken
XPathTokenDisambiguator::next() {
  if (_buffer.empty()) {
    return _token_source.next_token();
  } else {
    auto token = _buffer.front();
    _buffer.pop_front();
    return token;
  }
}

XPathToken
XPathTokenDisambiguator::next_token()
{
  auto token = next();

  if (token.is(XPathTokenType::String)) {
    auto next = _token_source.next_token();
    _buffer.push_back(next);
    if (next.is(XPathTokenType::LeftParen)) {
      return XPathToken(XPathTokenType::Function, token.string());
    } else if (next.is(XPathTokenType::DoubleColon)) {
      return XPathToken(XPathTokenType::Axis, token.string());
    }
  }

  return token;
}
