#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xpath-internal.h"

void
xpath_tokens_free(xpath_tokens_t *tokens)
{
  free(tokens->xpath);
  g_array_free(tokens->tokens, TRUE);
  free(tokens);
}

xpath_tokens_t *
xpath_tokenize(const char * const xpath)
{
  xpath_tokens_t *tokens;
  xpath_token_t token;
  const char *token_start = NULL;
  const char *current;

  tokens = malloc(sizeof(*tokens));
  tokens->xpath = strdup(xpath);
  tokens->tokens = g_array_new(FALSE, FALSE, sizeof(xpath_token_t));

#define ADD_TOKEN(_type)			\
  {						\
    token.type = _type;				\
    token.offset = current - xpath;		\
    g_array_append_val(tokens->tokens, token);	\
  }

#define FINISH_TEXT()				\
  if (token_start) {				\
    ADD_TOKEN(TEXT);				\
    token_start = NULL;				\
  }

  for (current = xpath; *current; current++) {
    if (*current == '/') {
      FINISH_TEXT();
      ADD_TOKEN(SLASH);
    } else if (*current == '[') {
      FINISH_TEXT();
      ADD_TOKEN(LBRACKET);
    } else if (*current == ']') {
      FINISH_TEXT();
      ADD_TOKEN(RBRACKET);
    } else if (*current == '(') {
      FINISH_TEXT();
      ADD_TOKEN(LPAREN);
    } else if (*current == ')') {
      FINISH_TEXT();
      ADD_TOKEN(RPAREN);
    } else if (*current == '"') {
      FINISH_TEXT();
      ADD_TOKEN(QUOTE);
    } else if (*current == '\'') {
      FINISH_TEXT();
      ADD_TOKEN(APOS);
    } else {
      if (token_start == NULL) {
	token_start = current;
      }
    }
  }

  FINISH_TEXT();

#undef FINISH_TEXT
#undef ADD_TOKEN

  return tokens;
}
