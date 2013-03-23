#ifndef XPATH_PARSER_TOKEN_SOURCE_H
#define XPATH_PARSER_TOKEN_SOURCE_H

#include "xpath-tokenizer.h"

class XPathParserTokenSource {
public:
  XPathParserTokenSource(XPathTokenSource &tokenizer);
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
