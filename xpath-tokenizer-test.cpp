#include "xpath-tokenizer.h"

#include "document.h"

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
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("world"));

  ASSERT_THAT(tokenizer, IsFinished());
}

TEST_F(XPathTokenizerTest, tokenizes_great_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/there/world");

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("hello"));

  ASSERT_TRUE(tokenizer.has_more_tokens());
  ASSERT_THAT(tokenizer.next_token(), IsStringToken("there"));

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

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
