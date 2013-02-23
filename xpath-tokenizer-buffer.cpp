#include "xpath-tokenizer-buffer.h"

XPathTokenizerBuffer::XPathTokenizerBuffer(XPathRawTokenSource &tokenizer) :
  _tokenizer(tokenizer), _token_is_saved(false), _token(XPathToken(""))
{
}

bool
XPathTokenizerBuffer::has_more_tokens()
{
  return _token_is_saved || _tokenizer.has_more_tokens();
}

XPathToken
XPathTokenizerBuffer::next_token()
{
  if (_token_is_saved) {
    _token_is_saved = false;
    return _token;
  }

  return _tokenizer.next_token();
}

XPathToken
XPathTokenizerBuffer::peek_token()
{
  if (! _token_is_saved) {
    _token = _tokenizer.next_token();
    _token_is_saved = true;
  }

  return _token;
}

bool
XPathTokenizerBuffer::next_token_is(XPathTokenType type)
{
  return has_more_tokens() && peek_token().is(type);
}
