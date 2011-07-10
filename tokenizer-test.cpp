#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

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

static void
STRNCMP_EQUAL_LOCATION(
  const char *expected, const char *actual, int len,
  const char *file, int line
)
{
  int res;

  res = strncmp(expected, actual, len);
  Utest::getTestResult()->countCheck();
  if (0 != res) {
    GString *str = g_string_new(NULL);
    g_string_printf(str, "Expected [%s] but got [%.*s]", expected, len, actual);
    FAIL_TEST_LOCATION(str->str, file, line);
    g_string_free(str, TRUE);
  }
}

static void
_NEXT_TOKEN_STRING(
  token_type_t type, // const char *type_name,
  const char *expected, tokenizer_t *tz,
  const char *file, int line
)
{
  int len;
  token_t tok;

  len = strlen(expected);
  tok = tokenizer_next(tz);

  CHECK_EQUAL_LOCATION(type, tok.type, file, line);
  CHECK_EQUAL_LOCATION(len, tok.value.string.len, file, line);
  STRNCMP_EQUAL_LOCATION(expected, tok.value.string.str, len, file, line);
}

#define NEXT_TOKEN_STRING(type, epected, tz) \
  _NEXT_TOKEN_STRING(type, epected, tz, __FILE__, __LINE__)

TEST(tokenize, tokenize_end)
{
  tz = tokenizer_new("");
  NEXT_TOKEN(END, tz);
}

TEST(tokenize, tokenize_space_space)
{
  tz = tokenizer_new(" ");
  NEXT_TOKEN(SPACE, tz);
}

TEST(tokenize, tokenize_space_tab)
{
  tz = tokenizer_new("	");
  NEXT_TOKEN(SPACE, tz);
}

TEST(tokenize, tokenize_space_lf)
{
  tz = tokenizer_new("\n");
  NEXT_TOKEN(SPACE, tz);
}

TEST(tokenize, tokenize_space_cr)
{
  tz = tokenizer_new("\r");
  NEXT_TOKEN(SPACE, tz);
}

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

TEST(tokenize, tokenize_eq)
{
  tz = tokenizer_new("=");
  NEXT_TOKEN(EQ, tz);
}

TEST(tokenize, tokenize_apos)
{
  tz = tokenizer_new("'");
  NEXT_TOKEN(APOS, tz);
}

TEST(tokenize, tokenize_quot)
{
  tz = tokenizer_new("\"");
  NEXT_TOKEN(QUOT, tz);
}

TEST(tokenize, tokenize_string)
{
  tz = tokenizer_new("hello");
  NEXT_TOKEN_STRING(STRING, "hello", tz);
}

TEST(tokenize, tokenize_two)
{
  tz = tokenizer_new("<>");
  NEXT_TOKEN(LT, tz);
  NEXT_TOKEN(GT, tz);
}

TEST(tokenize, tokenize_element)
{
  tz = tokenizer_new("<world/>");
  NEXT_TOKEN(LT, tz);
  NEXT_TOKEN_STRING(STRING, "world", tz);
  NEXT_TOKEN(SLASH, tz);
  NEXT_TOKEN(GT, tz);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
