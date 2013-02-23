#ifndef XPATH_RAW_TOKEN_SOURCE_H
#define XPATH_RAW_TOKEN_SOURCE_H

#include "xpath-token.h"

class XPathRawTokenSource {
public:
  virtual bool has_more_tokens() const = 0;
  virtual XPathToken next_token() = 0;
};

#endif
