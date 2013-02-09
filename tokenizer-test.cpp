#include "tokenizer.h"

#include "gmock/gmock.h"
#include <iostream>

class TokenizerTest : public ::testing::Test {
protected:
  tokenizer_t *tz = NULL;

  void TearDown(void)
  {
    if (tz) tokenizer_free(tz);
  }
};

#define NEXT_TOKEN(_type, _tz)                  \
{                                               \
  token_t _tok = tokenizer_next(_tz);           \
  ASSERT_EQ(_type, _tok.type);                  \
}                                               \

static void
NEXT_TOKEN_STRING(
  const char *expected,
  tokenizer_t *tz, string_type_t string_type
)
{
  token_t tok = tokenizer_next_string(tz, string_type);
  ASSERT_EQ(STRING, tok.type);

  size_t len = strlen(expected);
  ASSERT_EQ(len, tok.value.string.len);

  auto subset = std::string(tok.value.string.str, len);
  ASSERT_EQ(expected, subset);
}

TEST_F(TokenizerTest, tokenize_end)
{
  tz = tokenizer_new("");
  NEXT_TOKEN(END, tz);
}

TEST_F(TokenizerTest, tokenize_after_end)
{
  tz = tokenizer_new("");
  NEXT_TOKEN(END, tz);
  NEXT_TOKEN(END, tz);
}

TEST_F(TokenizerTest, tokenize_space_space)
{
  tz = tokenizer_new(" ");
  NEXT_TOKEN(SPACE, tz);
}

TEST_F(TokenizerTest, tokenize_space_tab)
{
  tz = tokenizer_new("	");
  NEXT_TOKEN(SPACE, tz);
}

TEST_F(TokenizerTest, tokenize_space_lf)
{
  tz = tokenizer_new("\n");
  NEXT_TOKEN(SPACE, tz);
}

TEST_F(TokenizerTest, tokenize_space_cr)
{
  tz = tokenizer_new("\r");
  NEXT_TOKEN(SPACE, tz);
}

TEST_F(TokenizerTest, tokenize_lt)
{
  tz = tokenizer_new("<");
  NEXT_TOKEN(LT, tz);
}

TEST_F(TokenizerTest, tokenize_gt)
{
  tz = tokenizer_new(">");
  NEXT_TOKEN(GT, tz);
}

TEST_F(TokenizerTest, tokenize_close_tag_start)
{
  tz = tokenizer_new("</");
  NEXT_TOKEN(CLOSE_TAG_START, tz);
}

TEST_F(TokenizerTest, tokenize_pi_start)
{
  tz = tokenizer_new("<?");
  NEXT_TOKEN(PI_START, tz);
}

TEST_F(TokenizerTest, tokenize_pi_end)
{
  tz = tokenizer_new("?>");
  NEXT_TOKEN(PI_END, tz);
}

TEST_F(TokenizerTest, tokenize_comment_start)
{
  tz = tokenizer_new("<!--");
  NEXT_TOKEN(COMMENT_START, tz);
}

TEST_F(TokenizerTest, tokenize_comment_text)
{
  tz = tokenizer_new("foo-bar-->");
  NEXT_TOKEN_STRING("foo-bar", tz, COMMENT_TEXT);
}

TEST_F(TokenizerTest, tokenize_comment_end)
{
  tz = tokenizer_new("-->");
  NEXT_TOKEN(COMMENT_END, tz);
}

TEST_F(TokenizerTest, tokenize_slash)
{
  tz = tokenizer_new("/");
  NEXT_TOKEN(SLASH, tz);
}

TEST_F(TokenizerTest, tokenize_eq)
{
  tz = tokenizer_new("=");
  NEXT_TOKEN(EQ, tz);
}

TEST_F(TokenizerTest, tokenize_apos)
{
  tz = tokenizer_new("'");
  NEXT_TOKEN(APOS, tz);
}

TEST_F(TokenizerTest, tokenize_quot)
{
  tz = tokenizer_new("\"");
  NEXT_TOKEN(QUOT, tz);
}

TEST_F(TokenizerTest, tokenize_amp)
{
  tz = tokenizer_new("&");
  NEXT_TOKEN(AMP, tz);
}

TEST_F(TokenizerTest, tokenize_char_ref)
{
  tz = tokenizer_new("&#");
  NEXT_TOKEN(CHAR_REF, tz);
}

TEST_F(TokenizerTest, tokenize_char_ref_hex)
{
  tz = tokenizer_new("&#x");
  NEXT_TOKEN(CHAR_REF_HEX, tz);
}

TEST_F(TokenizerTest, tokenize_semicolon)
{
  tz = tokenizer_new(";");
  NEXT_TOKEN(SEMICOLON, tz);
}

TEST_F(TokenizerTest, tokenize_attr_value_apos)
{
  tz = tokenizer_new("hello world'");
  NEXT_TOKEN_STRING("hello world", tz, ATTR_VALUE_APOS);
  NEXT_TOKEN(APOS, tz);
  NEXT_TOKEN(END, tz);
}

TEST_F(TokenizerTest, tokenize_attr_value_quot)
{
  tz = tokenizer_new("hello world\"");
  NEXT_TOKEN_STRING("hello world", tz, ATTR_VALUE_QUOT);
  NEXT_TOKEN(QUOT, tz);
  NEXT_TOKEN(END, tz);
}

TEST_F(TokenizerTest, tokenize_chardata_not_whitespace_only)
{
  token_t token;

  tz = tokenizer_new(" \thi\r\n");
  token = tokenizer_next_string(tz, CHARDATA);
  ASSERT_FALSE(token.value.string.whitespace_only);
}

TEST_F(TokenizerTest, tokenize_chardata_whitespace_only)
{
  token_t token;

  tz = tokenizer_new(" \t\n\r");
  token = tokenizer_next_string(tz, CHARDATA);
  ASSERT_TRUE(token.value.string.whitespace_only);
}

TEST_F(TokenizerTest, tokenize_chardata_lt)
{
  tz = tokenizer_new("a, 2<");
  NEXT_TOKEN_STRING("a, 2", tz, CHARDATA);
  NEXT_TOKEN(LT, tz);
}

TEST_F(TokenizerTest, tokenize_chardata_amp)
{
  tz = tokenizer_new("a, 2&");
  NEXT_TOKEN_STRING("a, 2", tz, CHARDATA);
  NEXT_TOKEN(AMP, tz);
}

TEST_F(TokenizerTest, tokenize_chardata_bad_start)
{
  token_t token;
  tz = tokenizer_new("<");
  token = tokenizer_next_string(tz, CHARDATA);
  ASSERT_EQ(LT, token.type);
}

TEST_F(TokenizerTest, tokenize_name)
{
  tz = tokenizer_new("_H0-.");
  NEXT_TOKEN_STRING("_H0-.", tz, NAME);
}

TEST_F(TokenizerTest, tokenize_integer)
{
  tz = tokenizer_new("13790");
  NEXT_TOKEN_STRING("13790", tz, INTEGER);
}

TEST_F(TokenizerTest, tokenize_hex)
{
  tz = tokenizer_new("aF09");
  NEXT_TOKEN_STRING("aF09", tz, HEX);
}

TEST_F(TokenizerTest, tokenize_two)
{
  tz = tokenizer_new("<>");
  NEXT_TOKEN(LT, tz);
  NEXT_TOKEN(GT, tz);
}

TEST_F(TokenizerTest, tokenize_element)
{
  tz = tokenizer_new("<world/>");
  NEXT_TOKEN(LT, tz);
  NEXT_TOKEN_STRING("world", tz, NAME);
  NEXT_TOKEN(SLASH, tz);
  NEXT_TOKEN(GT, tz);
}

TEST_F(TokenizerTest, tokenize_push)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_push(tz);
  NEXT_TOKEN(LT, tz);
}

class TokenizerContextTest : public ::testing::Test {
protected:
  tokenizer_t *tz = NULL;
  tokenizer_context_t context;

  void TearDown(void)
  {
    if (tz) tokenizer_free(tz);
    tokenizer_context_destroy(&context);
  }
};

TEST_F(TokenizerContextTest, empty)
{
  tz = tokenizer_new("");
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(0, context.column);
  ASSERT_STREQ("", context.string);
  ASSERT_EQ(0, context.offset);
}

TEST_F(TokenizerContextTest, basic)
{
  tz = tokenizer_new("<hello>");
  tokenizer_next(tz);
  tokenizer_next_string(tz, NAME);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(1, context.column);
  ASSERT_STREQ("<hello>", context.string);
  ASSERT_EQ(1, context.offset);
}

TEST_F(TokenizerContextTest, string)
{
  tz = tokenizer_new("hello>");
  tokenizer_next_string(tz, NAME);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(5, context.column);
  ASSERT_STREQ("hello>", context.string);
  ASSERT_EQ(5, context.offset);
}

TEST_F(TokenizerContextTest, newline)
{
  tz = tokenizer_new("\nworld");
  tokenizer_next(tz);
  tokenizer_next_string(tz, NAME);

  context = tokenizer_context(tz);
  ASSERT_EQ(1, context.line);
  ASSERT_EQ(0, context.column);
  ASSERT_STREQ("\nworld", context.string);
  ASSERT_EQ(1, context.offset);
}

TEST_F(TokenizerContextTest, push)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_next(tz);
  tokenizer_push(tz);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(0, context.column);
  ASSERT_STREQ("<>", context.string);
  ASSERT_EQ(0, context.offset);
}

TEST_F(TokenizerContextTest, push_string)
{
  tz = tokenizer_new("hello<>");
  tokenizer_next_string(tz, NAME);
  tokenizer_next(tz);
  tokenizer_next(tz);
  tokenizer_push(tz);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(5, context.column);
  ASSERT_STREQ("hello<>", context.string);
  ASSERT_EQ(5, context.offset);
}

TEST_F(TokenizerContextTest, push_next)
{
  tz = tokenizer_new("<>");
  tokenizer_next(tz);
  tokenizer_push(tz);
  tokenizer_next(tz);

  context = tokenizer_context(tz);
  ASSERT_EQ(0, context.line);
  ASSERT_EQ(0, context.column);
  ASSERT_STREQ("<>", context.string);
  ASSERT_EQ(0, context.offset);
}

TEST_F(TokenizerTest, token_name)
{
  ASSERT_STREQ(">", tokenizer_token_name(GT));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
