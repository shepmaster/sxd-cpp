#include "xpath-tokenizer.h"

#include "gmock/gmock.h"
#include "xpath-raw-token-source-test.h"
#include <iostream>

using testing::ElementsAre;

class XPathTokenizerTest : public ::testing::Test {
};

MATCHER(IsFinished, "no more tokens available") {
  *result_listener << "has more tokens";
  return ! arg.has_more_tokens();
}

TEST_F(XPathTokenizerTest, empty_string_has_no_tokens)
{
  XPathTokenizer tokenizer("");
  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_simple_string)
{
  XPathTokenizer tokenizer("hello");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("hello")));
}

TEST_F(XPathTokenizerTest, tokenizes_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/world");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("hello"),
                                                 XPathToken(XPathTokenType::Slash),
                                                 XPathToken("world")));
}

TEST_F(XPathTokenizerTest, tokenizes_great_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/there/world");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("hello"),
                                                 XPathToken(XPathTokenType::Slash),
                                                 XPathToken("there"),
                                                 XPathToken(XPathTokenType::Slash),
                                                 XPathToken("world")));
}

TEST_F(XPathTokenizerTest, tokenizes_axis_separator)
{
  XPathTokenizer tokenizer("::");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::DoubleColon)));
}

TEST_F(XPathTokenizerTest, tokenizes_axis_selector)
{
  XPathTokenizer tokenizer("hello::world");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("hello"),
                                                 XPathToken(XPathTokenType::DoubleColon),
                                                 XPathToken("world")));
}

TEST_F(XPathTokenizerTest, tokenizes_single_slash)
{
  XPathTokenizer tokenizer("/");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::Slash)));
}

TEST_F(XPathTokenizerTest, tokenizes_double_slash)
{
  XPathTokenizer tokenizer("//");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::DoubleSlash)));
}

TEST_F(XPathTokenizerTest, tokenizes_double_slash_separator)
{
  XPathTokenizer tokenizer("hello//world");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("hello"),
                                                 XPathToken(XPathTokenType::DoubleSlash),
                                                 XPathToken("world")));
}

TEST_F(XPathTokenizerTest, tokenizes_left_paren)
{
  XPathTokenizer tokenizer("(");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::LeftParen)));
}

TEST_F(XPathTokenizerTest, tokenizes_right_paren)
{
  XPathTokenizer tokenizer(")");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::RightParen)));
}

TEST_F(XPathTokenizerTest, tokenizes_at_sign)
{
  XPathTokenizer tokenizer("@");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::AtSign)));
}

TEST_F(XPathTokenizerTest, tokenizes_single_dot)
{
  XPathTokenizer tokenizer(".");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(".")));
}

TEST_F(XPathTokenizerTest, tokenizes_double_dot)
{
  XPathTokenizer tokenizer("..");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken("..")));
}

TEST_F(XPathTokenizerTest, tokenizes_integral_number)
{
  XPathTokenizer tokenizer("42");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(42)));
}

TEST_F(XPathTokenizerTest, tokenizes_decimal_number)
{
  XPathTokenizer tokenizer("42.42");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(42.42)));
}

TEST_F(XPathTokenizerTest, tokenizes_decimal_number_without_integral_part)
{
  XPathTokenizer tokenizer(".42");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(.42)));
}

TEST_F(XPathTokenizerTest, tokenizes_left_bracket)
{
  XPathTokenizer tokenizer("[");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::LeftBracket)));
}

TEST_F(XPathTokenizerTest, tokenizes_right_bracket)
{
  XPathTokenizer tokenizer("]");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::RightBracket)));
}

TEST_F(XPathTokenizerTest, tokenizes_apostrophe)
{
  XPathTokenizer tokenizer("'");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::Apostrophe)));
}

TEST_F(XPathTokenizerTest, tokenizes_double_quote)
{
  XPathTokenizer tokenizer("\"");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::DoubleQuote)));
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
