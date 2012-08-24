#include "xpath-parser.h"

#include "document.h"
#include "make-unique.h"
#include "step-child.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

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

TEST_GROUP(XPathParser)
{
  StubTokens tokens;
  XPathCreator creator;
  std::unique_ptr<XPathParser> parser;

  void setup() {
    parser = make_unique<XPathParser>(tokens, std::ref(creator));
  }
};

TEST(XPathParser, parses_string_as_child)
{
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  CHECK_EQUAL_C_INT(1, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  CHECK(child);
}

TEST(XPathParser, parses_two_strings_as_grandchild)
{
  tokens.push_back(XPathToken("hello"));
  tokens.push_back(XPathToken("world"));

  parser->parse();

  CHECK_EQUAL_C_INT(2, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  CHECK(child);

  child = dynamic_cast<StepChild *>(creator.saved_parts[1].get());
  CHECK(child);
}

TEST(XPathParser, parses_self_axis)
{
  tokens.push_back(XPathToken("self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  CHECK_EQUAL_C_INT(2, creator.saved_parts.size());

  auto *child = dynamic_cast<StepChild *>(creator.saved_parts[0].get());
  CHECK(child);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
