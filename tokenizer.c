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

int
tokenize_string(const char *offset, token_t *tok, const char *invalid)
{
  const char *tmp = offset;
  int len;

  tok->value.string.whitespace_only = TRUE;

  while (! strchr(invalid, *tmp)) {
    if (tok->value.string.whitespace_only &&
        ! strchr(" \t\n\r", *tmp))
    {
      tok->value.string.whitespace_only = FALSE;
    }
    tmp++;
  }
  len = tmp - offset;

  tok->type = STRING;
  tok->value.string.str = offset;
  tok->value.string.len = len;

  return len;
}

int
is_name_start_char(const char *offset)
{
  /* TODO: manage length*/
  gunichar *converted;
  gunichar value;

  if (*offset == '\0') return FALSE;

  /* Easy ASCII-only tests */
  if (*offset == ':') return TRUE;
  if (*offset >= 'A' && *offset <= 'Z') return TRUE;
  if (*offset == '_') return TRUE;
  if (*offset >= 'a' && *offset <= 'z') return TRUE;

  /* TODO: error checking */
  converted = g_utf8_to_ucs4(offset, 1, NULL, NULL, NULL);
  value = *converted;
  free(converted);

  /* More expensive Unicode checks */
  if (value >= 0xC0 && value <= 0xD6) return TRUE;
  if (value >= 0xD8 && value <= 0xF6) return TRUE;
  if (value >= 0xF8 && value <= 0x2FF) return TRUE;
  if (value >= 0x370 && value <= 0x37D) return TRUE;
  if (value >= 0x37F && value <= 0x1FFF) return TRUE;
  if (value >= 0x200C && value <= 0x200D) return TRUE;
  if (value >= 0x2070 && value <= 0x218F) return TRUE;
  if (value >= 0x2C00 && value <= 0x2FEF) return TRUE;
  if (value >= 0x3001 && value <= 0xD7FF) return TRUE;
  if (value >= 0xF900 && value <= 0xFDCF) return TRUE;
  if (value >= 0xFDF0 && value <= 0xFFFD) return TRUE;
  if (value >= 0x10000 && value <= 0xEFFFF) return TRUE;

  return FALSE;
}

int
is_name_char(const char *offset)
{
  /* TODO: manage length*/
  gunichar *converted;
  gunichar value;

  if (*offset == '\0') return FALSE;
  if (is_name_start_char(offset)) return TRUE;

  /* Easy ASCII-only tests */
  if (*offset == '-') return TRUE;
  if (*offset == '.') return TRUE;
  if (*offset >= '0' && *offset <= '9') return TRUE;

  /* TODO: error checking */
  converted = g_utf8_to_ucs4(offset, 1, NULL, NULL, NULL);
  value = *converted;
  free(converted);

  /* More expensive Unicode checks */
  if (value == 0xB7) return TRUE;
  if (value >= 0x0300 && value <= 0x036F) return TRUE;
  if (value >= 0x203F && value <= 0x2040) return TRUE;

  return FALSE;
}

int
is_digit(const char *c)
{
  return g_ascii_isdigit(*c);
}

int
is_hex(const char *c)
{
  return g_ascii_isxdigit(*c);
}

typedef int (*test_char_t)(const char *);

int
tokenize_string_fns(const char *offset, test_char_t first, test_char_t rest, token_t *tok, int *len)
{
  const char *tmp;

  if (! first(offset)) return FALSE;

  tmp = offset + 1; /* Already verified the first character */
  while (rest(tmp)) tmp++;
  *len = tmp - offset;

  tok->type = STRING;
  tok->value.string.str = offset;
  tok->value.string.len = *len;

  return TRUE;
}

int
tokenize_name_string(const char *offset, token_t *tok, int *len)
{
  return tokenize_string_fns(offset, is_name_start_char, is_name_char, tok, len);
}

int
tokenize_integer_string(const char *offset, token_t *tok, int *len)
{
  return tokenize_string_fns(offset, is_digit, is_digit, tok, len);
}

int
tokenize_hex_string(const char *offset, token_t *tok, int *len)
{
  return tokenize_string_fns(offset, is_hex, is_hex, tok, len);
}

token_t
tokenizer_next_string(tokenizer_t *tokenizer, string_type_t string_type)
{
  token_t tok;
  int len = 1;
  int newline = FALSE;
  const char *offset;
  int string;

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

  if (string_type == NAME && tokenize_name_string(offset, &tok, &len)) string = TRUE;
  else if (string_type == INTEGER && tokenize_integer_string(offset, &tok, &len)) string = TRUE;
  else if (string_type == HEX && tokenize_hex_string(offset, &tok, &len)) string = TRUE;
  else string = FALSE;

  if (string) {
    /* Do nothing */
  } else if (offset[0] == '\0') {
    tok.type = END;
  } else if (offset[0] == '<') {
    if (offset[1] == '?') {
      tok.type = PI_START;
      len = 2;
    } else if (offset[1] == '/') {
      tok.type = CLOSE_TAG_START;
      len = 2;
    } else {
      tok.type = LT;
    }
  } else if (offset[0] == '&') {
    if (offset[1] == '#') {
      if (offset[2] == 'x') {
        tok.type = CHAR_REF_HEX;
        len = 3;
      } else {
        tok.type = CHAR_REF;
        len = 2;
      }
    } else {
      tok.type = AMP;
    }
  } else if (string_type == ATTR_VALUE_QUOT && (*offset != '"')) {
    len = tokenize_string(offset, &tok, "\"");
  } else if (string_type == ATTR_VALUE_APOS && (*offset != '\'')) {
    len = tokenize_string(offset, &tok, "'");
  } else if (string_type == CHARDATA) {
    len = tokenize_string(offset, &tok, "<&");
    /* TODO: Check for ]]> */
  } else if (offset[0] == ' ' ||
             offset[0] == '\t' ||
             offset[0] == '\n' ||
             offset[0] == '\r') {
    tok.type = SPACE;
    if (offset[0] == '\n' ||
        offset[0] == '\r') {
      newline = TRUE;
    }
  } else if (offset[0] == '>') {
    tok.type = GT;
  } else if (offset[0] == '?' && offset[1] == '>') {
    tok.type = PI_END;
    len = 2;
  } else if (offset[0] == '/') {
    tok.type = SLASH;
  } else if (offset[0] == '=') {
    tok.type = EQ;
  } else if (offset[0] == '\'') {
    tok.type = APOS;
  } else if (offset[0] == '"') {
    tok.type = QUOT;
  } else if (offset[0] == ';') {
    tok.type = SEMICOLON;
  } else {
    abort();
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
  case CLOSE_TAG_START:
    return "</";
  case PI_START:
    return "<?";
  case PI_END:
    return "?>";
  case EQ:
    return "=";
  case APOS:
    return "'";
  case QUOT:
    return "\"";
  case AMP:
    return "&";
  case CHAR_REF:
    return "&#";
  case CHAR_REF_HEX:
    return "&#x";
  case SEMICOLON:
    return ";";
  }

  return "";
}
