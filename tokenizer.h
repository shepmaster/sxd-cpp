#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef int tokenizer_t;

typedef enum {
  LT,
  STRING,
  SLASH,
  GT,
  END
} token_t;

tokenizer_t *
tokenizer_new(const char *input);

void
tokenizer_free(tokenizer_t *tokenizer);

token_t
tokenizer_next(tokenizer_t *tokenizer);

#endif
