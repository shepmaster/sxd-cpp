#ifndef XPATH_INTERNAL_H
#define XPATH_INTERNAL_H

#include <glib.h>
#include "xpath.h"

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

typedef struct {
  char *xpath;
  GArray *tokens;
} xpath_tokens_t;

void
xpath_tokens_free(xpath_tokens_t *tokens);

xpath_tokens_t *
xpath_tokenize(const char * const xpath);

#endif
