#include "xpath-parser.h"

#include "document.h"
#include "make-unique.h"
#include "step-child.h"

#include "gmock/gmock.h"
#include <iostream>

struct StubTokens : public XPathTokenSource {
  XPathToken next_token() {
    return tokens[index++];
  }

  bool has_more_tokens() {
    return index < tokens.size();
  }

  void push_back(XPathToken token) {
    tokens.push_back(token);
  }

  std::vector<XPathToken> tokens;
  int index = 0;
};

struct XPathCreator {
  void operator()(std::vector<std::unique_ptr<XPathStep>> &&parts) {
    saved_parts = std::move(parts);
  };

  std::vector<std::unique_ptr<XPathStep>> saved_parts;
};

class XPathParserTest : public ::testing::Test {
protected:
  StubTokens tokens;
  XPathCreator creator;
  std::unique_ptr<XPathParser> parser;

  XPathParserTest() {
    parser = make_unique<XPathParser>(tokens, std::ref(creator));
  }
};

TEST_F(XPathParserTest, parses_string_as_child)
{
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  ASSERT_EQ(1, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  ASSERT_NE(nullptr, child);
}

TEST_F(XPathParserTest, parses_two_strings_as_grandchild)
{
  tokens.push_back(XPathToken("hello"));
  tokens.push_back(XPathToken("world"));

  parser->parse();

  ASSERT_EQ(2, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  ASSERT_NE(nullptr, child);

  child = dynamic_cast<StepChild *>(creator.saved_parts[1].get());
  ASSERT_NE(nullptr, child);
}

TEST_F(XPathParserTest, parses_self_axis)
{
  tokens.push_back(XPathToken("self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  ASSERT_EQ(2, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  ASSERT_NE(nullptr, child);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
