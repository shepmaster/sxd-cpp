#include "function-not.h"

#include "xpath-token-disambiguator.h"

#include "gmock/gmock.h"
#include "xpath-raw-token-source-test.h"
#include <iostream>

using testing::ElementsAre;
using testing::Values;

class XPathTokenDisambiguatorTest : public ::testing::Test {
protected:
  RawTokenProvider raw_tokenizer;
};

class NodeTestDisambiguatorTest : public XPathTokenDisambiguatorTest,
                                  public ::testing::WithParamInterface<std::string>
{};

TEST_P(NodeTestDisambiguatorTest, disambiguates_node_test_functions)
{
  auto name = GetParam();
  raw_tokenizer.add({
      XPathToken(name),
      XPathTokenType::LeftParen,
  });

  XPathTokenDisambiguator disambig(raw_tokenizer);

  ASSERT_THAT(all_tokens(disambig),
              ElementsAre(XPathToken(XPathTokenType::NodeTest, name),
                          XPathToken(XPathTokenType::LeftParen)));
}

INSTANTIATE_TEST_CASE_P(NodeNames,
                        NodeTestDisambiguatorTest,
                        Values("comment", "text", "processing-instruction", "node"));

TEST_F(XPathTokenDisambiguatorTest, name_followed_by_left_paren_becomes_function_name)
{
  raw_tokenizer.add({
      XPathToken("test"),
      XPathTokenType::LeftParen,
  });

  XPathTokenDisambiguator disambig(raw_tokenizer);

  ASSERT_THAT(all_tokens(disambig),
              ElementsAre(XPathToken(XPathTokenType::Function, "test"),
                          XPathToken(XPathTokenType::LeftParen)));
}

TEST_F(XPathTokenDisambiguatorTest, name_followed_by_double_colon_becomes_axis_name)
{
  raw_tokenizer.add({
      XPathToken("test"),
      XPathTokenType::DoubleColon,
  });

  XPathTokenDisambiguator disambig(raw_tokenizer);

  ASSERT_THAT(all_tokens(disambig),
              ElementsAre(XPathToken(XPathTokenType::Axis, "test"),
                          XPathToken(XPathTokenType::DoubleColon)));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
