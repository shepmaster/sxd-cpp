#ifndef XPATH_TOKENIZER_BUFFER_H
#define XPATH_TOKENIZER_BUFFER_H

#include "xpath-token-source.h"
#include "xpath-tokenizer.h"

class XPathTokenizerBuffer : public XPathTokenSource {
public:
  XPathTokenizerBuffer(XPathRawTokenSource &tokenizer);
  bool has_more_tokens() const;
  XPathToken next_token();
  bool next_token_is(XPathTokenType type);

private:
  XPathToken peek_token();

  XPathRawTokenSource &_tokenizer;
  bool _token_is_saved;
  XPathToken _token;
};

#endif
