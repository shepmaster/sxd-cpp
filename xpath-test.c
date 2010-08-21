#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xpath-internal.h"

void
dump_xpath_tokens(xpath_tokens_t *tokens)
{
  int i;
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *t = &g_array_index(tokens->tokens, xpath_token_t, i);
    printf("%d\n", t->start);
  }
}

#define assert_token(_tokens, _index, _type, _start)			\
  {									\
    xpath_token_t *__token;						\
    __token = &g_array_index(_tokens->tokens, xpath_token_t, _index);	\
    assert(_type == __token->type);					\
    assert(_start == __token->start);					\
}

static void
test_xpath_tokenize(void)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";
  tokens = xpath_tokenize(xpath);

  assert(strcmp(xpath, tokens->xpath) == 0);
  assert(5 == tokens->tokens->len);

  assert_token(tokens, 0, SLASH, 0);
  assert_token(tokens, 1, SLASH, 1);
  assert_token(tokens, 2, TEXT,  2);
  assert_token(tokens, 3, SLASH, 5);
  assert_token(tokens, 4, TEXT,  6);

  xpath_tokens_free(tokens);
}

#define assert_str(_tokens, _index, _str)		\
  {							\
    char *__str = xpath_tokens_string(_tokens, _index);	\
    assert(strcmp(__str, _str) == 0);			\
    free(__str);					\
  }							\

static void
test_xpath_tokens_string(void)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";

  tokens = xpath_tokenize(xpath);
  assert_str(tokens, 0, "/");
  assert_str(tokens, 1, "/");
  assert_str(tokens, 2, "one");
  assert_str(tokens, 3, "/");
  assert_str(tokens, 4, "two");

  xpath_tokens_free(tokens);
}

static void
test_xpath_compile_node(void)
{
  const char * const name = "one";
  xpath_compiled_t *compiled;

  compiled = xpath_compile(name);

  assert(1 == compiled->predicates->len);
  assert(XPATH_PREDICATE_ELEMENT == g_array_index(compiled->predicates, xpath_predicate_t, 0).type);
  assert(strcmp(name, g_array_index(compiled->predicates, xpath_predicate_t, 0).name) == 0);
}

int
main(int argc, char **argv)
{
  test_xpath_tokenize();
  test_xpath_tokens_string();
  test_xpath_compile_node();

  return EXIT_SUCCESS;
}
