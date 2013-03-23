#ifndef XPATH_TOKEN_SOURCE_H
#define XPATH_TOKEN_SOURCE_H

#include "xpath-token.h"

class XPathTokenSource {
public:
  virtual bool has_more_tokens() const = 0;

  /**
   * Returns and consumes the next token.
   */
  virtual XPathToken next_token() = 0;
};

#endif
