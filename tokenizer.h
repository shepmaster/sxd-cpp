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
  CLOSE_TAG_START,
  PI_START,
  PI_END,
  COMMENT_START,
  EQ,
  APOS,
  QUOT,
  AMP,
  CHAR_REF,
  CHAR_REF_HEX,
  SEMICOLON
} token_type_t;

typedef struct {
  token_type_t type;
  union {
    struct {
      const char *str;
      int len;
      int whitespace_only;
    } string;
  } value;
} token_t;

typedef struct {
  int line;
  int column;
  char *string;
  int offset;
} tokenizer_context_t;

typedef enum {
  NONE,
  ATTR_VALUE_APOS,
  ATTR_VALUE_QUOT,
  CHARDATA,
  NAME,
  INTEGER,
  HEX,
  COMMENT_TEXT
} string_type_t;

tokenizer_t *
tokenizer_new(const char *input);

void
tokenizer_free(tokenizer_t *tokenizer);

token_t
tokenizer_next(tokenizer_t *tokenizer);

token_t
tokenizer_next_string(tokenizer_t *tokenizer, string_type_t string_type);

token_t
tokenizer_current(tokenizer_t *tokenizer);

void
tokenizer_push(tokenizer_t *tokenizer);

tokenizer_context_t
tokenizer_context(tokenizer_t *tokenizer);

void
tokenizer_context_destroy(tokenizer_context_t *context);

const char *
tokenizer_token_name(token_type_t type);

#endif
