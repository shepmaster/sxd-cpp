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
  Document doc;
  Node *top_node;

  StubTokens tokens;
  XPathCreator creator;
  std::unique_ptr<XPathParser> parser;

  XPathParserTest() {
    top_node = doc.new_element("top-node");
    parser = make_unique<XPathParser>(tokens, std::ref(creator));
  }
};

MATCHER_P2(Selects, start_node, expected_node, "") {
  Nodeset result;
  arg->select_nodes(start_node, result);

  if (1 == result.count()) {
    return expected_node == result[0];
  }
  return false;
}

Node *
add_child(Node *top_node, std::string name) {
  Document *doc = top_node->document();
  Node *n = doc->new_element(name.c_str());
  top_node->append_child(n);
  return n;
}

TEST_F(XPathParserTest, parses_string_as_child)
{
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  ASSERT_EQ(1, creator.saved_parts.size());

  auto hello = add_child(top_node, "hello");
  ASSERT_THAT(creator.saved_parts[0], Selects(top_node, hello));
}

TEST_F(XPathParserTest, parses_two_strings_as_grandchild)
{
  tokens.push_back(XPathToken("hello"));
  tokens.push_back(XPathToken("world"));

  parser->parse();

  ASSERT_EQ(2, creator.saved_parts.size());

  auto hello = add_child(top_node, "hello");
  auto world = add_child(top_node, "world");
  ASSERT_THAT(creator.saved_parts[0], Selects(top_node, hello));
  ASSERT_THAT(creator.saved_parts[1], Selects(top_node, world));
}

TEST_F(XPathParserTest, parses_self_axis)
{
  tokens.push_back(XPathToken("self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("top-node"));

  parser->parse();

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(creator.saved_parts[0], Selects(top_node, top_node));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}