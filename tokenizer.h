#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct tokenizerS tokenizer_t;

typedef enum {
  END,
  SPACE,
  LT,
  STRING,
  SLASH,
  GT,
  EQ,
  APOS,
  QUOT
} token_type_t;

typedef struct {
  token_type_t type;
  union {
    struct {
      const char *str;
      int len;
    } string;
  } value;
} token_t;

typedef struct {
  int line;
  int column;
  char *string;
  int offset;
} tokenizer_context_t;

tokenizer_t *
tokenizer_new(const char *input);

void
tokenizer_free(tokenizer_t *tokenizer);

token_t
tokenizer_next(tokenizer_t *tokenizer);

token_t
tokenizer_current(tokenizer_t *tokenizer);

tokenizer_context_t
tokenizer_context(tokenizer_t *tokenizer);

void
tokenizer_context_destroy(tokenizer_context_t *context);

#endif
