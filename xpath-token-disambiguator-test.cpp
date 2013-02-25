#include "function-not.h"

#include "xpath-token-disambiguator.h"

#include "gmock/gmock.h"
#include "xpath-raw-token-source-test.h"
#include <iostream>

using testing::ElementsAre;

class XPathTokenDisambiguatorTest : public ::testing::Test {
protected:
  RawTokenProvider raw_tokenizer;
};

TEST_F(XPathTokenDisambiguatorTest, name_followed_by_left_paren_becomes_function_name)
{
  raw_tokenizer.add({
      XPathToken("test"),
      XPathTokenType::LeftParen,
  });

  XPathTokenDisambiguator disambig(raw_tokenizer);

  ASSERT_THAT(all_tokens(disambig),
              ElementsAre(XPathToken(XPathTokenType::FunctionName, "test"),
                          XPathToken(XPathTokenType::LeftParen)));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
