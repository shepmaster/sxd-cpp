#ifndef XPATH_TOKENS_H
#define XPATH_TOKENS_H

#include "xpath-internal.h"

typedef enum {
  SLASH,
  LBRACKET,
  RBRACKET,
  LPAREN,
  RPAREN,
  QUOTE,
  APOS,
  TEXT
} xpath_token_type_t;

typedef struct {
  xpath_token_type_t type;
  unsigned int start;
} xpath_token_t;

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
