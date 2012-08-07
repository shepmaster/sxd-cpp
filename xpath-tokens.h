#ifndef XPATH_TOKENS_H
#define XPATH_TOKENS_H

#include "xpath-internal.h"

class XPathTokens {
public:
  ~XPathTokens();
  XPathTokens(const char * const xpath);

  xpath_token_t operator[](int index);
  int size();
  char * string(int index);

private:
  char *xpath;
  std::vector<xpath_token_t> _tokens;

  void build_tokens();
};

#endif
