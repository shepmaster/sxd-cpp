#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "tokenizer.h"
}

TEST_GROUP(tokenize)
{
  tokenizer_t *tz;

  void teardown(void)
  {
    if (tz) tokenizer_free(tz);
  }
};

#define NEXT_TOKEN(_type, _tz)                  \
{                                               \
  token_t _tok = tokenizer_next(_tz);           \
  CHECK_EQUAL(_type, _tok.type);                \
}                                               \


TEST(tokenize, tokenize_lt)
{
  tz = tokenizer_new("<");
  NEXT_TOKEN(LT, tz);
}

TEST(tokenize, tokenize_gt)
{
  tz = tokenizer_new(">");
  NEXT_TOKEN(GT, tz);
}

TEST(tokenize, tokenize_slash)
{
  tz = tokenizer_new("/");
  NEXT_TOKEN(SLASH, tz);
}

TEST(tokenize, tokenize_string)
{
  token_t tok;

  tz = tokenizer_new("hello");
  NEXT_TOKEN(STRING, tz);
  tok = tokenizer_current(tz);
  STRCMP_EQUAL("hello", tok.value.string);
}

TEST(tokenize, tokenize_two)
{
  tz = tokenizer_new("<>");
  NEXT_TOKEN(LT, tz);
  NEXT_TOKEN(GT, tz);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
