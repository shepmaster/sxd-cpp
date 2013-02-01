#ifndef XPATH_TOKENIZER_H
#define XPATH_TOKENIZER_H

#include "xpath-token-source.h"

class XPathTokenizer {
public:
  XPathTokenizer(std::string xpath);
  bool has_more_tokens();
  XPathToken next_token();
private:
  std::string _xpath;
  size_t _start;
};

#endif
