#include "xpath-token-deabbreviator.h"

XPathTokenDeabbreviator::XPathTokenDeabbreviator(XPathRawTokenSource &token_source) :
  _token_source(token_source)
{}

bool
XPathTokenDeabbreviator::has_more_tokens() const {
  return ! _buffer.empty() || _token_source.has_more_tokens();
}

XPathToken
XPathTokenDeabbreviator::next_token() {
  if (_buffer.empty()) {
    auto token = _token_source.next_token();

    if (token.is(XPathTokenType::AtSign)) {
      _buffer.push_back(XPathToken("attribute"));
      _buffer.push_back(XPathTokenType::DoubleColon);
    } else if (token.is(XPathTokenType::DoubleSlash)) {
      _buffer.push_back(XPathToken(XPathTokenType::Slash));
      _buffer.push_back(XPathToken("descendant-or-self"));
      _buffer.push_back(XPathToken(XPathTokenType::DoubleColon));
      _buffer.push_back(XPathToken("node"));
      _buffer.push_back(XPathToken(XPathTokenType::LeftParen));
      _buffer.push_back(XPathToken(XPathTokenType::RightParen));
      _buffer.push_back(XPathToken(XPathTokenType::Slash));
    } else if (token.is(XPathTokenType::CurrentNode)) {
      _buffer.push_back(XPathToken("self"));
      _buffer.push_back(XPathToken(XPathTokenType::DoubleColon));
      _buffer.push_back(XPathToken("node"));
      _buffer.push_back(XPathToken(XPathTokenType::LeftParen));
      _buffer.push_back(XPathToken(XPathTokenType::RightParen));
    } else if (token.is(XPathTokenType::ParentNode)) {
      _buffer.push_back(XPathToken("parent"));
      _buffer.push_back(XPathToken(XPathTokenType::DoubleColon));
      _buffer.push_back(XPathToken("node"));
      _buffer.push_back(XPathToken(XPathTokenType::LeftParen));
      _buffer.push_back(XPathToken(XPathTokenType::RightParen));
    } else {
      return token;
    }
  }

  auto token = _buffer.front();
  _buffer.pop_front();
  return token;
}
