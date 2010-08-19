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
    printf("%d\n", t->offset);
  }
}

#define assert_type(_tokens, _index, _type) \
  assert(_type == g_array_index(_tokens->tokens, xpath_token_t, _index).type);

static void
test_xpath_tokenize(void)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";
  tokens = xpath_tokenize(xpath);

  assert(strcmp(xpath, tokens->xpath) == 0);
  assert(5 == tokens->tokens->len);

  assert_type(tokens, 0, SLASH);
  assert_type(tokens, 1, SLASH);
  assert_type(tokens, 2, TEXT);
  assert_type(tokens, 3, SLASH);
  assert_type(tokens, 4, TEXT);

  xpath_tokens_free(tokens);
}

int
main(int argc, char **argv)
{
  test_xpath_tokenize();

  return EXIT_SUCCESS;
}
