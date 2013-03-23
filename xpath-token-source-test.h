#ifndef XPATH_TOKEN_SOURCE_TEST_H
#define XPATH_TOKEN_SOURCE_TEST_H

#include "xpath-token-source.h"
#include "xpath-parsing-exceptions.h"

std::vector<XPathToken>
all_tokens(XPathTokenSource &tokenizer)
{
  std::vector<XPathToken> result;
  while (tokenizer.has_more_tokens()) {
    result.push_back(tokenizer.next_token());
  }
  return result;
}

struct TokenProvider : public XPathTokenSource {
  bool has_more_tokens() const {
    return index < tokens.size();
  }

  XPathToken next_token() {
    if (! has_more_tokens()) {
      throw NoMoreTokensAvailableException();
    }
    return tokens[index++];
  }

  void add(std::initializer_list<XPathToken> tokens) {
    for (auto token : tokens) {
      this->tokens.push_back(token);
    }
  }

  void add(XPathToken token) {
    add({token});
  }

  std::vector<XPathToken> tokens;
  int index = 0;
};

#endif
