#include "xpath-tokenizer.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathTokenizerTest : public ::testing::Test {
};

MATCHER(IsFinished, "no more tokens available") {
  *result_listener << "has more tokens";
  return ! arg.has_more_tokens();
}

MATCHER_P(IsStringToken, expected_string, "")
{
  return arg.is(XPathTokenType::String) && expected_string == arg.string();
}

MATCHER_P(IsNumberToken, expected_number, "")
{
  return arg.is(XPathTokenType::Number) && expected_number == arg.number();
}

MATCHER_P(IsType, expected_type, "")
{
  return arg.is(expected_type);
}

TEST_F(XPathTokenizerTest, empty_string_has_no_tokens)
{
  XPathTokenizer tokenizer("");
  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_simple_string)
{
  XPathTokenizer tokenizer("hello");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/world");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::Slash));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("world"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_great_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/there/world");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::Slash));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("there"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::Slash));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("world"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_axis_separator)
{
  XPathTokenizer tokenizer("::");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::DoubleColon));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_axis_selector)
{
  XPathTokenizer tokenizer("hello::world");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::DoubleColon));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("world"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_single_slash)
{
  XPathTokenizer tokenizer("/");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::Slash));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_double_slash)
{
  XPathTokenizer tokenizer("//");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::DoubleSlash));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_double_slash_separator)
{
  XPathTokenizer tokenizer("hello//world");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::DoubleSlash));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("world"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_left_paren)
{
  XPathTokenizer tokenizer("(");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::LeftParen));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_right_paren)
{
  XPathTokenizer tokenizer(")");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::RightParen));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_at_sign)
{
  XPathTokenizer tokenizer("@");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::AtSign));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_single_dot)
{
  XPathTokenizer tokenizer(".");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("."));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_double_dot)
{
  XPathTokenizer tokenizer("..");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken(".."));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_integral_number)
{
  XPathTokenizer tokenizer("42");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsNumberToken(42));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_decimal_number)
{
  XPathTokenizer tokenizer("42.42");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsNumberToken(42.42));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_decimal_number_without_integral_part)
{
  XPathTokenizer tokenizer(".42");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsNumberToken(.42));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_left_bracket)
{
  XPathTokenizer tokenizer("[");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::LeftBracket));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_right_bracket)
{
  XPathTokenizer tokenizer("]");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::RightBracket));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_apostrophe)
{
  XPathTokenizer tokenizer("'");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::Apostrophe));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_double_quote)
{
  XPathTokenizer tokenizer("\"");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsType(XPathTokenType::DoubleQuote));

  ASSERT_THAT(tokenizer, IsFinished());
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
