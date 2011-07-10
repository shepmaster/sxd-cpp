#include <stdio.h>
#include <stdlib.h>

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

  if (tokenizer->current[0] == '<') {
    tok.type = LT;
  } else if (tokenizer->current[0] == '>') {
    tok.type = GT;
  } else if (tokenizer->current[0] == '/') {
    tok.type = SLASH;
  } else {
    tok.type = STRING;
    tok.value.string = tokenizer->current;
  }

  tokenizer->current++;

  tokenizer->current_token = tok;
  return tok;
}

token_t
tokenizer_current(tokenizer_t *tokenizer)
{
  return tokenizer->current_token;
}
