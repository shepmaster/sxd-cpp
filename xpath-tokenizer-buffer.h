#ifndef XPATH_TOKENIZER_BUFFER_H
#define XPATH_TOKENIZER_BUFFER_H

#include "xpath-tokenizer.h"

class XPathTokenizerBuffer {
public:
  XPathTokenizerBuffer(XPathTokenSource &tokenizer);
  bool has_more_tokens() const;
  XPathToken next_token();
  bool next_token_is(XPathTokenType type);
  XPathToken consume(XPathTokenType type);

private:
  XPathToken peek_token();

  XPathTokenSource &_tokenizer;
  bool _token_is_saved;
  XPathToken _token;
};

#endif
