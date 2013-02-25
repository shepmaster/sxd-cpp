#include "function-not.h"

#include "xpath-token-deabbreviator.h"
#include "xpath-tokenizer.h"

#include "gmock/gmock.h"
#include "xpath-raw-token-source-test.h"
#include <iostream>

using testing::ElementsAre;

class XPathTokenDeabbreviatorTest : public ::testing::Test {
protected:
  RawTokenProvider raw_tokenizer;
};

TEST_F(XPathTokenDeabbreviatorTest, converts_at_sign_to_attribute_axis)
{
  raw_tokenizer.add(XPathTokenType::AtSign);

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken("attribute"),
                                               XPathToken(XPathTokenType::DoubleColon)));
}

TEST_F(XPathTokenDeabbreviatorTest, converts_double_slash_to_descendant_or_self)
{
  raw_tokenizer.add(XPathTokenType::DoubleSlash);

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken(XPathTokenType::Slash),
                                               XPathToken("descendant-or-self"),
                                               XPathToken(XPathTokenType::DoubleColon),
                                               XPathToken("node"),
                                               XPathToken(XPathTokenType::LeftParen),
                                               XPathToken(XPathTokenType::RightParen),
                                               XPathToken(XPathTokenType::Slash)));
}

TEST_F(XPathTokenDeabbreviatorTest, converts_current_node_to_self_node)
{
  raw_tokenizer.add(XPathTokenType::CurrentNode);

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken("self"),
                                               XPathToken(XPathTokenType::DoubleColon),
                                               XPathToken("node"),
                                               XPathToken(XPathTokenType::LeftParen),
                                               XPathToken(XPathTokenType::RightParen)));
}

TEST_F(XPathTokenDeabbreviatorTest, converts_parent_node_to_parent_node)
{
  raw_tokenizer.add(XPathTokenType::ParentNode);

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken("parent"),
                                               XPathToken(XPathTokenType::DoubleColon),
                                               XPathToken("node"),
                                               XPathToken(XPathTokenType::LeftParen),
                                               XPathToken(XPathTokenType::RightParen)));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
