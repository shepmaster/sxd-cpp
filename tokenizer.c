#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "tokenizer.h"

typedef struct {
  int valid;
  const char *offset;
  token_t token;
  tokenizer_context_t context;
} state_t;

struct tokenizerS {
  const char *input;
  state_t *to_use;
  state_t previous;
  state_t current;
  tokenizer_context_t next_context;
};

tokenizer_t *
tokenizer_new(const char *input)
{
  tokenizer_t *tokenizer;
  tokenizer = calloc(sizeof(*tokenizer), 1);
  tokenizer->input = input;
  tokenizer->current.offset = input;
  tokenizer->to_use = &tokenizer->current;
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
  return tokenizer_next_string(tokenizer, NONE);
}

token_t
tokenizer_next_string(tokenizer_t *tokenizer, string_type_t attr_value)
{
  token_t tok;
  int len = 1;
  int newline = FALSE;
  const char *offset;

  if (tokenizer->to_use == &tokenizer->previous) {
    tokenizer->to_use = &tokenizer->current;
    return tokenizer_current(tokenizer);
  }

  if (END == tokenizer->to_use->token.type &&
      tokenizer->to_use->valid)
  {
    return tokenizer_current(tokenizer);
  }

  offset = tokenizer->current.offset;
  tokenizer->previous = tokenizer->current;

  if (offset[0] == '\0') {
    tok.type = END;
  } else if (attr_value != NONE) {
    const char *tmp;
    tok.type = STRING;
    tok.value.string.str = offset;
    tmp = offset;

    switch (attr_value) {
    case ATTR_VALUE_APOS:
      while (*tmp != '\'') tmp++;
      break;
    case ATTR_VALUE_QUOT:
      while (*tmp != '"') tmp++;
      break;
    case NONE:
      abort();
    }

    len = tmp - offset;
    tok.value.string.len = len;
  } else if (offset[0] == ' ' ||
             offset[0] == '\t' ||
             offset[0] == '\n' ||
             offset[0] == '\r') {
    tok.type = SPACE;
    if (offset[0] == '\n' ||
        offset[0] == '\r') {
      newline = TRUE;
    }
  } else if (offset[0] == '<') {
    tok.type = LT;
  } else if (offset[0] == '>') {
    tok.type = GT;
  } else if (offset[0] == '/') {
    tok.type = SLASH;
  } else if (offset[0] == '=') {
    tok.type = EQ;
  } else if (offset[0] == '\'') {
    tok.type = APOS;
  } else if (offset[0] == '"') {
    tok.type = QUOT;
  } else {
    const char *tmp;
    tok.type = STRING;
    tok.value.string.str = offset;
    tmp = offset;
    /* Using ascii function here is wrong */
    /* Needs to be a Name? */
    while (g_ascii_isalpha(*tmp)) tmp++;
    len = tmp - offset;
    tok.value.string.len = len;
  }

  tokenizer->current.valid = TRUE;
  tokenizer->current.context = tokenizer->next_context;
  tokenizer->current.token = tok;
  tokenizer->current.offset += len;

  /* This seems ugly */
  if (newline) {
    tokenizer->next_context.line++;
    tokenizer->next_context.column = 0;
  } else {
    tokenizer->next_context.column += len;
  }
  tokenizer->next_context.offset += len;

  return tok;
}

token_t
tokenizer_current(tokenizer_t *tokenizer)
{
  return tokenizer->to_use->token;
}

void
tokenizer_push(tokenizer_t *tokenizer)
{
  if (tokenizer->to_use == &tokenizer->previous) {
    abort();
  } else {
    tokenizer->to_use = &tokenizer->previous;
  }
}

tokenizer_context_t
tokenizer_context(tokenizer_t *tokenizer)
{
  tokenizer_context_t context;

  context = tokenizer->to_use->context;
  context.string = g_strdup(tokenizer->input);

  return context;
}

void
tokenizer_context_destroy(tokenizer_context_t *context)
{
  free(context->string);
}

const char *
tokenizer_token_name(token_type_t type)
{
  switch (type) {
  case END:
    return "End-of-input";
  case SPACE:
    return "whitespace";
  case LT:
    return "<";
  case STRING:
    return "[string]";
  case SLASH:
    return "/";
  case GT:
    return ">";
  case EQ:
    return "=";
  case APOS:
    return "'";
  case QUOT:
    return "\"";
  }

  return "";
}
