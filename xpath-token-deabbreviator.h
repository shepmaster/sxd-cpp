#ifndef XPATH_TOKEN_DEABBREVIATOR_H
#define XPATH_TOKEN_DEABBREVIATOR_H

#include "xpath-raw-token-source.h"

#include <deque>

class XPathTokenDeabbreviator : public XPathRawTokenSource {
public:
  XPathTokenDeabbreviator(XPathRawTokenSource &tokenizer);

  bool has_more_tokens() const;
  XPathToken next_token();

private:
  XPathRawTokenSource &_token_source;
  std::deque<XPathToken> _buffer;
};

#endif
