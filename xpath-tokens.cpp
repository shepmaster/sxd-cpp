#include "xpath-tokens.h"

XPathTokens::XPathTokens(const char * const xpath)
{
  this->xpath = strdup(xpath);
}

XPathTokens::~XPathTokens()
{
  free(xpath);
}

void
XPathTokens::build_tokens()
{
  xpath_token_t token;
  const char *token_start = NULL;
  const char *current;

  if (! _tokens.empty()) {
    return;
  }

  #define ADD_TOKEN(_type, _start)                \
    {                                             \
      token.type = _type;                         \
      token.start = _start - xpath;               \
      _tokens.push_back(token);                   \
    }

  #define FINISH_TEXT()                           \
    if (token_start) {                            \
      ADD_TOKEN(TEXT, token_start);               \
      token_start = NULL;                         \
    }

  for (current = xpath; *current; current++) {
    if (*current == '/') {
      FINISH_TEXT();
      ADD_TOKEN(SLASH, current);
    } else if (*current == '[') {
      FINISH_TEXT();
      ADD_TOKEN(LBRACKET, current);
    } else if (*current == ']') {
      FINISH_TEXT();
      ADD_TOKEN(RBRACKET, current);
    } else if (*current == '(') {
      FINISH_TEXT();
      ADD_TOKEN(LPAREN, current);
    } else if (*current == ')') {
      FINISH_TEXT();
      ADD_TOKEN(RPAREN, current);
    } else if (*current == '"') {
      FINISH_TEXT();
      ADD_TOKEN(QUOTE, current);
    } else if (*current == '\'') {
      FINISH_TEXT();
      ADD_TOKEN(APOS, current);
    } else {
      if (token_start == NULL) {
        token_start = current;
      }
    }
  }

  FINISH_TEXT();

  #undef FINISH_TEXT
  #undef ADD_TOKEN
}

xpath_token_t
XPathTokens::operator[](int index)
{
  build_tokens();
  return _tokens[index];
}

int
XPathTokens::size()
{
  build_tokens();
  return _tokens.size();
}

char *
XPathTokens::string(int index)
{
  xpath_token_t *token;
  char *start_of_string;

  build_tokens();
  token = &_tokens[index];
  start_of_string = xpath + token->start;

  if (index + 1 < _tokens.size()) {
    xpath_token_t *next_token;
    int len;

    next_token = &_tokens[index + 1];
    len = next_token->start - token->start;

    return g_strndup(start_of_string, len);
  } else {
    return strdup(start_of_string);
  }
}
