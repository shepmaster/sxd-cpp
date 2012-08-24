#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-tokenizer.h"

TEST_GROUP(XPathTokenizer)
{
};

TEST(XPathTokenizer, empty_string_has_no_tokens)
{
  XPathTokenizer tokenizer("");
  CHECK(! tokenizer.has_more_tokens());
}

TEST(XPathTokenizer, tokenizes_simple_string)
{
  XPathTokenizer tokenizer("hello");

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok = tokenizer.next_token();
  CHECK_EQUAL("hello", tok.string());

  CHECK(! tokenizer.has_more_tokens());
}

TEST(XPathTokenizer, tokenizes_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/world");

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok = tokenizer.next_token();
  CHECK_EQUAL("hello", tok.string());

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok2 = tokenizer.next_token();
  CHECK_EQUAL("world", tok2.string());

  CHECK(! tokenizer.has_more_tokens());
}

TEST(XPathTokenizer, tokenizes_great_grandchild_selector)
{
  XPathTokenizer tokenizer("hello/there/world");

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok = tokenizer.next_token();
  CHECK_EQUAL("hello", tok.string());

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok2 = tokenizer.next_token();
  CHECK_EQUAL("there", tok2.string());

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok3 = tokenizer.next_token();
  CHECK_EQUAL("world", tok3.string());

  CHECK(! tokenizer.has_more_tokens());
}

TEST(XPathTokenizer, tokenizes_axis_separator)
{
  XPathTokenizer tokenizer("::");

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok = tokenizer.next_token();
  CHECK(tok.is(XPathTokenType::DoubleColon));

  CHECK(! tokenizer.has_more_tokens());
}

TEST(XPathTokenizer, tokenizes_axis_selector)
{
  XPathTokenizer tokenizer("hello::world");

  CHECK(tokenizer.has_more_tokens());
  XPathToken tok = tokenizer.next_token();
  CHECK_EQUAL("hello", tok.string());

  XPathToken tok2 = tokenizer.next_token();
  CHECK(tok2.is(XPathTokenType::DoubleColon));

  XPathToken tok3 = tokenizer.next_token();
  CHECK_EQUAL("world", tok3.string());

  CHECK(! tokenizer.has_more_tokens());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
