#include "function-not.h"

#include "xpath-token-deabbreviator.h"
#include "xpath-tokenizer.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

class XPathTokenDeabbreviatorTest : public ::testing::Test {
protected:
  std::vector<XPathToken> all_tokens(XPathRawTokenSource &tokenizer) {
    std::vector<XPathToken> result;
    while (tokenizer.has_more_tokens()) {
      result.push_back(tokenizer.next_token());
    }
    return result;
  }
};

TEST_F(XPathTokenDeabbreviatorTest, converts_at_sign_to_attribute_axis)
{
  XPathTokenizer raw_tokenizer("@");

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken("attribute"),
                                               XPathToken(XPathTokenType::DoubleColon)));
}

TEST_F(XPathTokenDeabbreviatorTest, converts_double_slash_to_descendant_or_self)
{
  XPathTokenizer raw_tokenizer("//");

  XPathTokenDeabbreviator deabbrv(raw_tokenizer);

  ASSERT_THAT(all_tokens(deabbrv), ElementsAre(XPathToken("descendant-or-self"),
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
