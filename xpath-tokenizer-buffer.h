#ifndef XPATH_TOKENIZER_BUFFER_H
#define XPATH_TOKENIZER_BUFFER_H

#include "xpath-token-source.h"
#include "xpath-tokenizer.h"

class XPathTokenizerBuffer : public XPathTokenSource {
public:
  XPathTokenizerBuffer(XPathTokenizer tokenizer);
  bool has_more_tokens();
  XPathToken next_token();
  bool next_token_is(XPathTokenType type);

private:
  XPathToken peek_token();

  XPathTokenizer _tokenizer;
  bool _token_is_saved;
  XPathToken _token;
};

#endif
