#ifndef MOCK_XPATH_TOKEN_SOURCE_H
#define MOCK_XPATH_TOKEN_SOURCE_H

#include "xpath-token-source.h"

class MockTokenSource : public XPathTokenSource
{
public:
  MOCK_CONST_METHOD0(has_more_tokens, bool());
  MOCK_METHOD0(next_token, XPathToken());
};

#endif
