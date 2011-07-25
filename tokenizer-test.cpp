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
  const char *expected,
  tokenizer_t *tz, string_type_t string_type,
  const char *file, int line
)
{
  int len;
  token_t tok;

  len = strlen(expected);
  tok = tokenizer_next_string(tz, string_type);

  CHECK_EQUAL_LOCATION(STRING, tok.type, file, line);
  CHECK_EQUAL_LOCATION(len, tok.value.string.len, file, line);
  STRNCMP_EQUAL_LOCATION(expected, tok.value.string.str, len, file, line);
}

#define NEXT_TOKEN_STRING(expected, tz, string_type)                    \
  _NEXT_TOKEN_STRING(expected, tz, string_type, __FILE__, __LINE__)

TEST(tokenize, tokenize_end)
{
  tz = tokenizer_new("");
  NEXT_TOKEN(END, tz);
}

TEST(tokenize, tokenize_after_end)
{
  tz = tokenizer_new("");
  NEXT_TOKEN(END, tz);
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

TEST(tokenize, tokenize_attr_value_apos)
{
  tz = tokenizer_new("hello world'");
  NEXT_TOKEN_STRING("hello world", tz, ATTR_VALUE_APOS);
  NEXT_TOKEN(APOS, tz);
  NEXT_TOKEN(END, tz);
}

TEST(tokenize, tokenize_attr_value_quot)
{
  tz = tokenizer_new("hello world\"");
  NEXT_TOKEN_STRING("hello world", tz, ATTR_VALUE_QUOT);
  NEXT_TOKEN(QUOT, tz);
  NEXT_TOKEN(END, tz);
}

TEST(tokenize, tokenize_chardata_lt)
{
  tz = tokenizer_new("a, 2<");
  NEXT_TOKEN_STRING("a, 2", tz, CHARDATA);
  NEXT_TOKEN(LT, tz);
}

TEST(tokenize, tokenize_chardata_amp)
{
  tz = tokenizer_new("a, 2&");
  NEXT_TOKEN_STRING("a, 2", tz, CHARDATA);
  NEXT_TOKEN(AMP, tz);
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
  NEXT_TOKEN_STRING("world", tz, NONE);
  NEXT_TOKEN(SLASH, tz);
  NEXT_TOKEN(GT, tz);
}

TEST(tokenize, tokenize_push)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_push(tz);
  NEXT_TOKEN(LT, tz);
}

TEST_GROUP(tokenize_context)
{
  tokenizer_t *tz;
  tokenizer_context_t context;

  void teardown(void)
  {
    if (tz) tokenizer_free(tz);
    tokenizer_context_destroy(&context);
  }
};

TEST(tokenize_context, empty)
{
  tz = tokenizer_new("");
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(0, context.column);
  STRCMP_EQUAL("", context.string);
  CHECK_EQUAL(0, context.offset);
}

TEST(tokenize_context, basic)
{
  tz = tokenizer_new("<hello>");
  tokenizer_next(tz);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(1, context.column);
  STRCMP_EQUAL("<hello>", context.string);
  CHECK_EQUAL(1, context.offset);
}

TEST(tokenize_context, string)
{
  tz = tokenizer_new("hello>");
  tokenizer_next(tz);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(5, context.column);
  STRCMP_EQUAL("hello>", context.string);
  CHECK_EQUAL(5, context.offset);
}

TEST(tokenize_context, newline)
{
  tz = tokenizer_new("\nworld");
  tokenizer_next(tz);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(1, context.line);
  CHECK_EQUAL(0, context.column);
  STRCMP_EQUAL("\nworld", context.string);
  CHECK_EQUAL(1, context.offset);
}

TEST(tokenize_context, push)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_next(tz);
  tokenizer_push(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(0, context.column);
  STRCMP_EQUAL("<>", context.string);
  CHECK_EQUAL(0, context.offset);
}

TEST(tokenize_context, push_string)
{
  tz = tokenizer_new("hello<>");
  tokenizer_next(tz);
  tokenizer_next(tz);
  tokenizer_next(tz);
  tokenizer_push(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(5, context.column);
  STRCMP_EQUAL("hello<>", context.string);
  CHECK_EQUAL(5, context.offset);
}

TEST(tokenize_context, push_next)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_push(tz);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  CHECK_EQUAL(0, context.line);
  CHECK_EQUAL(0, context.column);
  STRCMP_EQUAL("<>", context.string);
  CHECK_EQUAL(0, context.offset);
}

TEST(tokenize, token_name)
{
  STRCMP_EQUAL(">", tokenizer_token_name(GT));
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
