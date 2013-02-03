#ifndef XPATH_TOKENIZER_H
#define XPATH_TOKENIZER_H

#include "xpath-token-source.h"
#include <iostream>

class XPathTokenizer {
public:
  XPathTokenizer(std::string xpath);
  bool has_more_tokens() const;
  XPathToken next_token();
private:
  std::string _xpath;
  size_t _start;

  friend std::ostream& operator<<(std::ostream &, const XPathTokenizer &);
};

std::ostream& operator<<(std::ostream &, const XPathTokenizer &);

#endif
