#ifndef XPATH_TOKEN_SOURCE_H
#define XPATH_TOKEN_SOURCE_H

#include "xpath-token.h"

class XPathTokenSource {
public:
  virtual bool has_more_tokens() = 0;

  /**
   * Returns and consumes the next token.
   */
  virtual XPathToken next_token() = 0;

  /**
   * Tests if the next token is the specified type. Is false if there
   * are no more tokens.
   */
  virtual bool next_token_is(XPathTokenType type) = 0;
};

#endif
