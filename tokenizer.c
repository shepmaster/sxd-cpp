#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "tokenizer.h"

struct tokenizerS {
  const char *input;
  const char *current;
  token_t current_token;
  tokenizer_context_t current_context;
  tokenizer_context_t next_context;
};

tokenizer_t *
tokenizer_new(const char *input)
{
  tokenizer_t *tokenizer;
  tokenizer = calloc(sizeof(*tokenizer), 1);
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
  int newline = FALSE;

  tokenizer->current_context = tokenizer->next_context;

  if (tokenizer->current[0] == '\0') {
    tok.type = END;
  } else if (tokenizer->current[0] == ' ' ||
             tokenizer->current[0] == '\t' ||
             tokenizer->current[0] == '\n' ||
             tokenizer->current[0] == '\r') {
    tok.type = SPACE;
    if (tokenizer->current[0] == '\n' ||
        tokenizer->current[0] == '\r') {
      newline = TRUE;
    }
  } else if (tokenizer->current[0] == '<') {
    tok.type = LT;
  } else if (tokenizer->current[0] == '>') {
    tok.type = GT;
  } else if (tokenizer->current[0] == '/') {
    tok.type = SLASH;
  } else if (tokenizer->current[0] == '=') {
    tok.type = EQ;
  } else if (tokenizer->current[0] == '\'') {
    tok.type = APOS;
  } else if (tokenizer->current[0] == '"') {
    tok.type = QUOT;
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

  /* This seems ugly */
  if (newline) {
    tokenizer->next_context.line++;
    tokenizer->next_context.column = 0;
  } else {
    tokenizer->next_context.column += len;
  }
  tokenizer->next_context.offset += len;

  tokenizer->current_token = tok;
  return tok;
}

token_t
tokenizer_current(tokenizer_t *tokenizer)
{
  return tokenizer->current_token;
}

tokenizer_context_t
tokenizer_context(tokenizer_t *tokenizer)
{
  tokenizer_context_t context;

  context = tokenizer->current_context;
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
