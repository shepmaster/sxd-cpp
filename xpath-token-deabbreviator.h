#ifndef XPATH_TOKEN_DEABBREVIATOR_H
#define XPATH_TOKEN_DEABBREVIATOR_H

#include "xpath-token-source.h"

#include <deque>

class XPathTokenDeabbreviator : public XPathTokenSource {
public:
  XPathTokenDeabbreviator(XPathTokenSource &tokenizer);

  bool has_more_tokens() const;
  XPathToken next_token();

private:
  XPathTokenSource &_token_source;
  std::deque<XPathToken> _buffer;
};

#endif
