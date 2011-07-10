#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "tokenizer.h"

struct tokenizerS {
  const char *input;
  const char *current;
  token_t current_token;
};

tokenizer_t *
tokenizer_new(const char *input)
{
  tokenizer_t *tokenizer;
  tokenizer = calloc(sizeof(&tokenizer), 1);
  tokenizer->input = input;
  tokenizer->current = input;
  return tokenizer;
}

void
tokenizer_free(tokenizer_t *tokenizer)
{
  free(tokenizer);
}

token_t
tokenizer_next(tokenizer_t *tokenizer)
{
  token_t tok;
  int len = 1;

  if (tokenizer->current[0] == '<') {
    tok.type = LT;
  } else if (tokenizer->current[0] == '>') {
    tok.type = GT;
  } else if (tokenizer->current[0] == '/') {
    tok.type = SLASH;
  } else {
    const char *tmp;
    tok.type = STRING;
    tok.value.string.str = tokenizer->current;
    tmp = tokenizer->current;
    /* Using ascii function here is wrong */
    /* Needs to be a Name? */
    while (g_ascii_isalpha(*tmp)) tmp++;
    len = tmp - tokenizer->current;
    tok.value.string.len = len;
  }

  tokenizer->current += len;

  tokenizer->current_token = tok;
  return tok;
}

token_t
tokenizer_current(tokenizer_t *tokenizer)
{
  return tokenizer->current_token;
}
