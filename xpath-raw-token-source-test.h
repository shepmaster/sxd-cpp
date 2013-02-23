#ifndef XPATH_RAW_TOKEN_SOURCE_TEST_H
#define XPATH_RAW_TOKEN_SOURCE_TEST_H

std::vector<XPathToken>
all_tokens(XPathRawTokenSource &tokenizer)
{
  std::vector<XPathToken> result;
  while (tokenizer.has_more_tokens()) {
    result.push_back(tokenizer.next_token());
  }
  return result;
}

#endif
