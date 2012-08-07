#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-tokens.h"

TEST_GROUP(xpath)
{};

void
dump_xpath_tokens(XPathTokens &tokens)
{
  unsigned int i;
  for (i = 0; i < tokens.size(); i++) {
    xpath_token_t t = tokens[i];
    printf("%d\n", t.start);
  }
}

#define CHECK_token(_tokens, _index, _type, _start)     \
  {                                                     \
    xpath_token_t __token;                              \
    __token = tokens[_index];                           \
    CHECK_EQUAL(_type, __token.type);                   \
    CHECK_EQUAL(_start, __token.start);                 \
  }

TEST(xpath, tokenize)
{
  const char * const xpath = "//one/two";
  XPathTokens tokens(xpath);

  CHECK_EQUAL_C_INT(5, tokens.size());

  CHECK_token(tokens, 0, SLASH, 0);
  CHECK_token(tokens, 1, SLASH, 1);
  CHECK_token(tokens, 2, TEXT,  2);
  CHECK_token(tokens, 3, SLASH, 5);
  CHECK_token(tokens, 4, TEXT,  6);
}

#define CHECK_str(_tokens, _index, _str)        \
  {                                             \
    char *__str = _tokens.string(_index);       \
    STRCMP_EQUAL(_str, __str);                  \
    free(__str);                                \
  }                                             \

TEST(xpath, tokens_string)
{
  const char * const xpath = "//one/two";
  XPathTokens tokens(xpath);

  CHECK_str(tokens, 0, "/");
  CHECK_str(tokens, 1, "/");
  CHECK_str(tokens, 2, "one");
  CHECK_str(tokens, 3, "/");
  CHECK_str(tokens, 4, "two");
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
