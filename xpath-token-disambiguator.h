#ifndef XPATH_TOKEN_DISAMBIGUATOR_H
#define XPATH_TOKEN_DISAMBIGUATOR_H

#include "xpath-raw-token-source.h"

#include <deque>

class XPathTokenDisambiguator : public XPathRawTokenSource {
public:
  XPathTokenDisambiguator(XPathRawTokenSource &token_source);

  bool has_more_tokens() const;
  XPathToken next_token();

private:
  XPathToken next();

  XPathRawTokenSource &_token_source;
  std::deque<XPathToken> _buffer;
};

#endif
