#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "tokenizer.h"
}

TEST_GROUP(tokenize)
{};

TEST(tokenize, tokenize_lt)
{
  tokenizer_t *tz;
  token_t tok;

  tz = tokenizer_new("<");
  tok = tokenizer_next(tz);
  CHECK_EQUAL(LT, tok);

  tokenizer_free(tz);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
