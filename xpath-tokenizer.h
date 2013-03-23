#ifndef XPATH_TOKENIZER_H
#define XPATH_TOKENIZER_H

#include "xpath-token-source.h"
#include <iostream>

class XPathTokenizer : public XPathTokenSource {
public:
  XPathTokenizer(std::string xpath);
  bool has_more_tokens() const;
  XPathToken next_token();

private:
  XPathToken tokenize_literal(char quote_char);
  void consume_whitespace();
  XPathToken raw_next_token();

  std::string _xpath;
  size_t _start;
  bool _prefer_recognition_of_operator_names;

  friend std::ostream& operator<<(std::ostream &, const XPathTokenizer &);
};

std::ostream& operator<<(std::ostream &, const XPathTokenizer &);

#endif
