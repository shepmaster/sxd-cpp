#ifndef XPATH_TOKEN_DISAMBIGUATOR_H
#define XPATH_TOKEN_DISAMBIGUATOR_H

#include "xpath-token-source.h"

#include <deque>

class XPathTokenDisambiguator : public XPathTokenSource {
public:
  XPathTokenDisambiguator(XPathTokenSource &token_source);

  bool has_more_tokens() const;
  XPathToken next_token();

private:
  XPathToken next();

  XPathTokenSource &_token_source;
  std::deque<XPathToken> _buffer;
};

#endif
