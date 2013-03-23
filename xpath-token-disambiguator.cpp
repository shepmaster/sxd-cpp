#include "xpath-token-disambiguator.h"

#include <set>

XPathTokenDisambiguator::XPathTokenDisambiguator(XPathTokenSource &token_source) :
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

std::set<std::string> node_test_names =
  { "comment", "text", "processing-instruction", "node" };

XPathToken
XPathTokenDisambiguator::next_token()
{
  auto token = next();

  if (token.is(XPathTokenType::String)) {
    if (! _token_source.has_more_tokens()) {
      return token;
    }
    auto next = _token_source.next_token();
    _buffer.push_back(next);
    if (next.is(XPathTokenType::LeftParen)) {
      if (node_test_names.find(token.string()) != node_test_names.end()) {
        return XPathToken(XPathTokenType::NodeTest, token.string());
      } else {
        return XPathToken(XPathTokenType::Function, token.string());
      }
    } else if (next.is(XPathTokenType::DoubleColon)) {
      return XPathToken(XPathTokenType::Axis, token.string());
    }
  }

  return token;
}
