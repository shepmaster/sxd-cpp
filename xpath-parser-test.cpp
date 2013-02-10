#include "xpath-parser.h"

#include "document.h"
#include "make-unique.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

struct StubTokens : public XPathTokenSource {
  bool next_token_is(XPathTokenType type) {
    return has_more_tokens() && tokens[index].is(type);
  }

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

  Nodeset
  apply_xpath_step(int index, Node *context_node)
  {
    Nodeset result;
    creator.saved_parts[index]->select_nodes(context_node, result);
    return result;
  }
};

Node *
add_child(Node *top_node, std::string name) {
  Document *doc = top_node->document();
  Node *n = doc->new_element(name.c_str());
  top_node->append_child(n);
  return n;
}

Node *
add_text_node(Node *parent, std::string value) {
  Document *doc = parent->document();
  Node *n = doc->new_text_node(value.c_str());
  parent->append_child(n);
  return n;
}

TEST_F(XPathParserTest, parses_string_as_child)
{
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  ASSERT_EQ(1, creator.saved_parts.size());

  auto hello = add_child(top_node, "hello");
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(hello));
}

TEST_F(XPathParserTest, parses_two_strings_as_grandchild)
{
  tokens.push_back(XPathToken("hello"));
  tokens.push_back(XPathToken("world"));

  parser->parse();

  ASSERT_EQ(2, creator.saved_parts.size());

  auto hello = add_child(top_node, "hello");
  auto world = add_child(top_node, "world");
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(hello));
  ASSERT_THAT(apply_xpath_step(1, top_node), ElementsAre(world));
}

TEST_F(XPathParserTest, parses_self_axis)
{
  tokens.push_back(XPathToken("self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("top-node"));

  parser->parse();

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(top_node));
}

TEST_F(XPathParserTest, parses_parent_axis)
{
  tokens.push_back(XPathToken("parent"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("top-node"));

  parser->parse();

  auto hello = add_child(top_node, "hello");
  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, hello), ElementsAre(top_node));
}

TEST_F(XPathParserTest, parses_descendant_axis)
{
  tokens.push_back(XPathToken("descendant"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("two"));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(two));
}

TEST_F(XPathParserTest, parses_child_with_same_name_as_an_axis)
{
  tokens.push_back(XPathToken("self"));

  parser->parse();

  auto self = add_child(top_node, "self");
  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(self));
}

TEST_F(XPathParserTest, single_dot_abbreviation_selects_itself)
{
  tokens.push_back(XPathToken("."));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, text), ElementsAre(text));
}

TEST_F(XPathParserTest, parses_double_slash)
{
  // This is a bit dubious - can you really say '//' by itself?
  tokens.push_back(XPathToken(XPathTokenType::DoubleSlash));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, one), ElementsAre(one, two));
}

TEST_F(XPathParserTest, parses_text_node_test)
{
  tokens.push_back(XPathToken("text"));
  tokens.push_back(XPathToken(XPathTokenType::LeftParen));
  tokens.push_back(XPathToken(XPathTokenType::RightParen));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, one), ElementsAre(text));
}

TEST_F(XPathParserTest, parses_axis_and_node_test)
{
  tokens.push_back(XPathToken("self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("text"));
  tokens.push_back(XPathToken(XPathTokenType::LeftParen));
  tokens.push_back(XPathToken(XPathTokenType::RightParen));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_EQ(1, creator.saved_parts.size());
  ASSERT_THAT(apply_xpath_step(0, text), ElementsAre(text));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
