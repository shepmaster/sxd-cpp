#include "xpath-tokenizer.h"

#include "xpath-parsing-exceptions.h"

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

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathTokenType::CurrentNode));
}

TEST_F(XPathTokenizerTest, tokenizes_double_dot)
{
  XPathTokenizer tokenizer("..");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathTokenType::ParentNode));
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

TEST_F(XPathTokenizerTest, tokenizes_apostrophe_literal)
{
  XPathTokenizer tokenizer("'hello!'");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::Literal, "hello!")));
}

TEST_F(XPathTokenizerTest, tokenizes_double_quote_literal)
{
  XPathTokenizer tokenizer("\"1.23\"");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::Literal, "1.23")));
}

TEST_F(XPathTokenizerTest, tokenizes_plus_sign)
{
  XPathTokenizer tokenizer("+");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::PlusSign)));
}

TEST_F(XPathTokenizerTest, tokenizes_minus_sign)
{
  XPathTokenizer tokenizer("-");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::MinusSign)));
}

TEST_F(XPathTokenizerTest, tokenizes_equal_sign)
{
  XPathTokenizer tokenizer("=");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::Equal)));
}

TEST_F(XPathTokenizerTest, tokenizes_not_equal_sign)
{
  XPathTokenizer tokenizer("!=");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::NotEqual)));
}

TEST_F(XPathTokenizerTest, tokenizes_less_than)
{
  XPathTokenizer tokenizer("<");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(XPathTokenType::LessThan)));
}

TEST_F(XPathTokenizerTest, special_preceding_token_forces_named_operator_and)
{
  XPathTokenizer tokenizer("1andz2");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(1),
                                                 XPathToken(XPathTokenType::And),
                                                 XPathToken("z2")));
}

TEST_F(XPathTokenizerTest, special_preceding_token_forces_named_operator_or)
{
  XPathTokenizer tokenizer("2oror");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(2),
                                                 XPathToken(XPathTokenType::Or),
                                                 XPathToken("or")));
}

TEST_F(XPathTokenizerTest, special_preceding_token_forces_named_operator_mod)
{
  XPathTokenizer tokenizer("3moddiv");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(3),
                                                 XPathToken(XPathTokenType::Remainder),
                                                 XPathToken("div")));
}

TEST_F(XPathTokenizerTest, special_preceding_token_forces_named_operator_div)
{
  XPathTokenizer tokenizer("1divz2");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(1),
                                                 XPathToken(XPathTokenType::Divide),
                                                 XPathToken("z2")));
}

TEST_F(XPathTokenizerTest, special_preceding_token_forces_named_operator_multiply)
{
  XPathTokenizer tokenizer("1*2");

  ASSERT_THAT(all_tokens(tokenizer), ElementsAre(XPathToken(1),
                                                 XPathToken(XPathTokenType::Multiply),
                                                 XPathToken(2)));
}

TEST_F(XPathTokenizerTest, exception_thrown_when_no_more_tokens_available)
{
  XPathTokenizer tokenizer("");

  ASSERT_THROW(tokenizer.next_token(), NoMoreTokensAvailableException);
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
