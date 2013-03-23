#include "xpath-parser-token-source.h"
#include "xpath-parsing-exceptions.h"

XPathParserTokenSource::XPathParserTokenSource(XPathTokenSource &tokenizer) :
  _tokenizer(tokenizer), _token_is_saved(false), _token(XPathToken(""))
{
}

bool
XPathParserTokenSource::has_more_tokens() const
{
  return _token_is_saved || _tokenizer.has_more_tokens();
}

XPathToken
XPathParserTokenSource::next_token()
{
  if (_token_is_saved) {
    _token_is_saved = false;
    return _token;
  }

  return _tokenizer.next_token();
}

XPathToken
XPathParserTokenSource::peek_token()
{
  if (! _token_is_saved) {
    _token = _tokenizer.next_token();
    _token_is_saved = true;
  }

  return _token;
}

bool
XPathParserTokenSource::next_token_is(XPathTokenType type)
{
  return has_more_tokens() && peek_token().is(type);
}

XPathToken
XPathParserTokenSource::consume(XPathTokenType type) {
  auto token = next_token();
  if (! token.is(type)) {
    throw UnexpectedTokenException();
  }
  return token;
}
