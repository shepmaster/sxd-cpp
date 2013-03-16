#ifndef MOCK_XPATH_RAW_TOKEN_SOURCE_H
#define MOCK_XPATH_RAW_TOKEN_SOURCE_H

#include "xpath-raw-token-source.h"

class MockRawTokenSource : public XPathRawTokenSource
{
public:
  MOCK_CONST_METHOD0(has_more_tokens, bool());
  MOCK_METHOD0(next_token, XPathToken());
};

#endif
