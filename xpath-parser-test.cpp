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

struct XPathSaver : public XPathCreator {
  void add_step(std::unique_ptr<XPathStep> step) {
    saved_parts.push_back(std::move(step));
  };

  std::vector<std::unique_ptr<XPathStep>> saved_parts;
};

class XPathParserTest : public ::testing::Test {
protected:
  Document doc;
  Node *top_node;

  StubTokens tokens;
  XPathSaver creator;
  std::unique_ptr<XPathParser> parser;

  XPathParserTest() {
    top_node = doc.new_element("top-node");
    parser = make_unique<XPathParser>(tokens, std::ref(creator));
  }

  Node *add_child(Node *parent, std::string name) {
    Node *n = doc.new_element(name.c_str());
    parent->append_child(n);
    return n;
  }

  Node *add_attribute(Node *element, std::string name, std::string value) {
    return dynamic_cast<Element *>(element)->set_attribute(name, value);
  }

  Node *add_text_node(Node *parent, std::string value) {
    Node *n = doc.new_text_node(value.c_str());
    parent->append_child(n);
    return n;
  }

  Nodeset apply_xpath_step(int index, Node *context_node) {
    Nodeset result;
    creator.saved_parts[index]->select_nodes(context_node, result);
    return result;
  }

  int number_of_steps() {
    return creator.saved_parts.size();
  }
};

TEST_F(XPathParserTest, parses_string_as_child)
{
  tokens.push_back(XPathToken("hello"));

  parser->parse();

  ASSERT_EQ(1, number_of_steps());

  auto hello = add_child(top_node, "hello");
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(hello));
}

TEST_F(XPathParserTest, parses_two_strings_as_grandchild)
{
  tokens.push_back(XPathToken("hello"));
  tokens.push_back(XPathToken("world"));

  parser->parse();

  ASSERT_EQ(2, number_of_steps());

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

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(top_node));
}

TEST_F(XPathParserTest, parses_parent_axis)
{
  tokens.push_back(XPathToken("parent"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("top-node"));

  parser->parse();

  auto hello = add_child(top_node, "hello");
  ASSERT_EQ(1, number_of_steps());
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

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(two));
}

TEST_F(XPathParserTest, parses_descendant_or_self_axis)
{
  tokens.push_back(XPathToken("descendant-or-self"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("*"));

  parser->parse();

  auto one = add_child(top_node, "one");
  add_child(one, "two");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_EQ(2, apply_xpath_step(0, one).size());
}

TEST_F(XPathParserTest, parses_attribute_axis)
{
  tokens.push_back(XPathToken("attribute"));
  tokens.push_back(XPathToken(XPathTokenType::DoubleColon));
  tokens.push_back(XPathToken("*"));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto attr = add_attribute(one, "hello", "world");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, one), ElementsAre(attr));
}

TEST_F(XPathParserTest, parses_child_with_same_name_as_an_axis)
{
  tokens.push_back(XPathToken("self"));

  parser->parse();

  auto self = add_child(top_node, "self");
  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, top_node), ElementsAre(self));
}

TEST_F(XPathParserTest, single_dot_abbreviation_selects_itself)
{
  tokens.push_back(XPathToken("."));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, text), ElementsAre(text));
}

TEST_F(XPathParserTest, double_slash_abbreviation_selects_itself_and_children)
{
  // This is a bit dubious - can you really say '//' by itself?
  tokens.push_back(XPathToken(XPathTokenType::DoubleSlash));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, one), ElementsAre(one, two));
}

TEST_F(XPathParserTest, double_slash_abbreviation_can_select_text_nodes)
{
  // This is a bit dubious - can you really say '//' by itself?
  tokens.push_back(XPathToken(XPathTokenType::DoubleSlash));

  parser->parse();

  auto one = add_child(top_node, "one");
  add_text_node(one, "text");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_EQ(2, apply_xpath_step(0, one).size());
}

TEST_F(XPathParserTest, parses_node_node_test)
{
  tokens.push_back(XPathToken("node"));
  tokens.push_back(XPathToken(XPathTokenType::LeftParen));
  tokens.push_back(XPathToken(XPathTokenType::RightParen));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, one), ElementsAre(two));
}

TEST_F(XPathParserTest, parses_text_node_test)
{
  tokens.push_back(XPathToken("text"));
  tokens.push_back(XPathToken(XPathTokenType::LeftParen));
  tokens.push_back(XPathToken(XPathTokenType::RightParen));

  parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_EQ(1, number_of_steps());
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

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, text), ElementsAre(text));
}

TEST_F(XPathParserTest, at_sign_abbreviation_selects_attributes)
{
  tokens.push_back(XPathToken(XPathTokenType::AtSign));
  tokens.push_back(XPathToken("name"));

  parser->parse();

  auto element = add_child(top_node, "element");
  auto attr = add_attribute(element, "name", "value");

  ASSERT_EQ(1, number_of_steps());
  ASSERT_THAT(apply_xpath_step(0, element), ElementsAre(attr));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
