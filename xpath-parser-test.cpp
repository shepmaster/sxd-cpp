#include "xpath-parser.h"

#include "document.h"
#include "xpath-core-function-library.h"
#include "xpath-evaluation-context-impl.h"
#include "make-unique.h"
#include "xpath-parsing-exceptions.h"

#include "gmock/gmock.h"
#include "xpath-raw-token-source-test.h"
#include <iostream>

using testing::ElementsAre;

struct StubTokens : public XPathTokenSource {
  bool has_more_tokens() const { return raw_tokens.has_more_tokens(); }
  XPathToken next_token() { return raw_tokens.next_token(); }
  void add(std::initializer_list<XPathToken> tokens) { raw_tokens.add(tokens); }
  void add(XPathToken token) { raw_tokens.add(token); }

  bool next_token_is(XPathTokenType type) {
    return raw_tokens.has_more_tokens() &&
      raw_tokens.tokens[raw_tokens.index].is(type);
  }

  RawTokenProvider raw_tokens;
};

#include "xpath-variable-bindings-hash.h"

class XPathParserTest : public ::testing::Test {
protected:
  Document doc;
  Element *top_node;

  StubTokens tokens;
  std::unique_ptr<XPathParser> parser;

  XPathFunctionLibrary functions;
  XPathVariableBindingsHash variables;

  void SetUp() {
    XPathCoreFunctionLibrary::register_functions(functions);
    top_node = doc.new_element("top-node");
    parser = make_unique<XPathParser>(tokens);
  }

  Element *add_child(Element *parent, std::string name) {
    auto n = doc.new_element(name);
    parent->append_child(n);
    return n;
  }

  Attribute *add_attribute(Element *element, std::string name, std::string value) {
    return element->set_attribute(name, value);
  }

  TextNode *add_text_node(Element *parent, std::string value) {
    auto tn = doc.new_text_node(value);
    parent->append_child(tn);
    return tn;
  }

  XPathValue
  evaluate(const std::unique_ptr<XPathExpression> &expr) {
    return evaluate_on(expr, nullptr);
  }

  XPathValue
  evaluate_on(const std::unique_ptr<XPathExpression> &expr, Node *node) {
    Nodeset empty_nodeset;
    XPathEvaluationContextImpl context(node, empty_nodeset, functions, variables);
    return expr->evaluate(context);
  }
};

TEST_F(XPathParserTest, parses_string_as_child)
{
  tokens.add(XPathToken("hello"));

  auto expr = parser->parse();

  auto hello = add_child(top_node, "hello");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(hello));
}

TEST_F(XPathParserTest, parses_two_strings_as_grandchild)
{
  tokens.add({
      XPathToken("hello"),
      XPathToken(XPathTokenType::Slash),
      XPathToken("world")
  });

  auto expr = parser->parse();

  auto hello = add_child(top_node, "hello");
  auto world = add_child(hello, "world");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(world));
}

TEST_F(XPathParserTest, parses_self_axis)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "self"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("top-node")
  });

  auto expr = parser->parse();

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(top_node));
}

TEST_F(XPathParserTest, parses_parent_axis)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "parent"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("top-node")
  });

  auto expr = parser->parse();

  auto hello = add_child(top_node, "hello");
  ASSERT_THAT(evaluate_on(expr, hello).nodeset(), ElementsAre(top_node));
}

TEST_F(XPathParserTest, parses_descendant_axis)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "descendant"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("two")
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(two));
}

TEST_F(XPathParserTest, parses_descendant_or_self_axis)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "descendant-or-self"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("*")
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_THAT(evaluate_on(expr, one).nodeset(), ElementsAre(one, two));
}

TEST_F(XPathParserTest, parses_attribute_axis)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "attribute"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("*")
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto attr = add_attribute(one, "hello", "world");

  ASSERT_THAT(evaluate_on(expr, one).nodeset(), ElementsAre(attr));
}

TEST_F(XPathParserTest, parses_child_with_same_name_as_an_axis)
{
  tokens.add(XPathToken("self"));

  auto expr = parser->parse();

  auto self = add_child(top_node, "self");
  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(self));
}

TEST_F(XPathParserTest, parses_node_node_test)
{
  tokens.add({
      XPathToken(XPathTokenType::NodeTest, "node"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen)
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto two = add_child(one, "two");

  ASSERT_THAT(evaluate_on(expr, one).nodeset(), ElementsAre(two));
}

TEST_F(XPathParserTest, parses_text_node_test)
{
  tokens.add({
      XPathToken(XPathTokenType::NodeTest, "text"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen)
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_THAT(evaluate_on(expr, one).nodeset(), ElementsAre(text));
}

TEST_F(XPathParserTest, parses_axis_and_node_test)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "self"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken(XPathTokenType::NodeTest, "text"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen)
  });

  auto expr = parser->parse();

  auto one = add_child(top_node, "one");
  auto text = add_text_node(one, "text");

  ASSERT_THAT(evaluate_on(expr, text).nodeset(), ElementsAre(text));
}

TEST_F(XPathParserTest, numeric_predicate_selects_indexed_node)
{
  tokens.add({
      XPathToken("*"),
      XPathToken(XPathTokenType::LeftBracket),
      XPathToken(2),
      XPathToken(XPathTokenType::RightBracket)
  });

  auto expr = parser->parse();

  add_child(top_node, "first");
  auto second = add_child(top_node, "second");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(second));
}

TEST_F(XPathParserTest, string_literal)
{
  tokens.add(XPathToken(XPathTokenType::Literal, "string"));

  auto expr = parser->parse();

  ASSERT_EQ("string", evaluate(expr).string());
}

TEST_F(XPathParserTest, true_function_predicate_selects_all_nodes)
{
  tokens.add({
      XPathToken("*"),
      XPathToken(XPathTokenType::LeftBracket),
      XPathToken(XPathTokenType::Function, "true"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
      XPathToken(XPathTokenType::RightBracket)
  });

  auto expr = parser->parse();

  auto first = add_child(top_node, "first");
  auto second = add_child(top_node, "second");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre(first, second));
}

TEST_F(XPathParserTest, false_function_predicate_selects_no_nodes)
{
  tokens.add({
      XPathToken("*"),
      XPathToken(XPathTokenType::LeftBracket),
      XPathToken(XPathTokenType::Function, "false"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
      XPathToken(XPathTokenType::RightBracket)
  });

  auto expr = parser->parse();

  add_child(top_node, "first");
  add_child(top_node, "second");

  ASSERT_THAT(evaluate_on(expr, top_node).nodeset(), ElementsAre());
}

TEST_F(XPathParserTest, functions_accept_arguments)
{
  tokens.add({
      XPathToken(XPathTokenType::Function, "not"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::Function, "true"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
      XPathToken(XPathTokenType::RightParen),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, numeric_literal)
{
  tokens.add({
      XPathToken(3.2)
  });

  auto expr = parser->parse();

  ASSERT_EQ(3.2, evaluate(expr).number());
}

TEST_F(XPathParserTest, addition_of_two_numbers)
{
  tokens.add({
      XPathToken(1.1),
      XPathToken(XPathTokenType::PlusSign),
      XPathToken(2.2)
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(3.3, evaluate(expr).number());
}

TEST_F(XPathParserTest, addition_of_multiple_numbers)
{
  tokens.add({
      XPathToken(1.1),
      XPathToken(XPathTokenType::PlusSign),
      XPathToken(2.2),
      XPathToken(XPathTokenType::PlusSign),
      XPathToken(3.3)
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(6.6, evaluate(expr).number());
}

TEST_F(XPathParserTest, subtraction_of_two_numbers)
{
  tokens.add({
      XPathToken(1.1),
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(2.2),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(-1.1, evaluate(expr).number());
}

TEST_F(XPathParserTest, additive_expression_is_left_associative)
{
  tokens.add({
      XPathToken(1.1),
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(2.2),
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(3.3),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(-4.4, evaluate(expr).number());
}

TEST_F(XPathParserTest, multiplication_of_two_numbers)
{
  tokens.add({
      XPathToken(1.1),
      XPathToken(XPathTokenType::Multiply),
      XPathToken(2.2),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(2.42, evaluate(expr).number());
}

TEST_F(XPathParserTest, division_of_two_numbers)
{
  tokens.add({
      XPathToken(7.1),
      XPathToken(XPathTokenType::Divide),
      XPathToken(0.1),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(71, evaluate(expr).number());
}

TEST_F(XPathParserTest, remainder_of_two_numbers)
{
  tokens.add({
      XPathToken(7.1),
      XPathToken(XPathTokenType::Remainder),
      XPathToken(3),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(1.1, evaluate(expr).number());
}

TEST_F(XPathParserTest, unary_negation)
{
  tokens.add({
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(7.2),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(-7.2, evaluate(expr).number());
}

TEST_F(XPathParserTest, repeated_unary_negation)
{
  tokens.add({
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(XPathTokenType::MinusSign),
      XPathToken(7.2),
  });

  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(-7.2, evaluate(expr).number());
}

TEST_F(XPathParserTest, top_level_function_call)
{
  tokens.add({
      XPathToken(XPathTokenType::Function, "true"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
  });

  auto expr = parser->parse();

  ASSERT_EQ(true, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, or_expression)
{
  tokens.add({
      XPathToken(XPathTokenType::Function, "true"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
      XPathToken(XPathTokenType::Or),
      XPathToken(XPathTokenType::Function, "false"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen),
  });

  auto expr = parser->parse();

  ASSERT_EQ(true, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, and_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::And),
      XPathToken(0),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, equality_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::Equal),
      XPathToken(1.1),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, inequality_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::NotEqual),
      XPathToken(1.2),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, less_than_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::LessThan),
      XPathToken(1.2),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, less_than_or_equal_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::LessThanOrEqual),
      XPathToken(1.2),
  });

  auto expr = parser->parse();

  ASSERT_EQ(true, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, greater_than_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::GreaterThan),
      XPathToken(1.2),
  });

  auto expr = parser->parse();

  ASSERT_EQ(false, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, greater_than_or_equal_expression)
{
  tokens.add({
      XPathToken(1.2),
      XPathToken(XPathTokenType::GreaterThanOrEqual),
      XPathToken(1.2),
  });

  auto expr = parser->parse();

  ASSERT_EQ(true, evaluate(expr).boolean());
}

TEST_F(XPathParserTest, variable_reference)
{
  tokens.add({
      XPathToken(XPathTokenType::DollarSign),
      XPathToken("variable-name"),
  });

  variables.set("variable-name", 12.3);
  auto expr = parser->parse();

  ASSERT_DOUBLE_EQ(12.3, evaluate(expr).number());
}

TEST_F(XPathParserTest, unknown_axis_is_reported_as_an_error)
{
  tokens.add({
      XPathToken(XPathTokenType::Axis, "bad-axis"),
      XPathToken(XPathTokenType::DoubleColon),
      XPathToken("*")
  });

  ASSERT_THROW(parser->parse(), InvalidXPathAxisException);
}

TEST_F(XPathParserTest, unknown_node_test_is_reported_as_an_error)
{
  tokens.add({
      XPathToken(XPathTokenType::NodeTest, "bad-node-test"),
      XPathToken(XPathTokenType::LeftParen),
      XPathToken(XPathTokenType::RightParen)
  });

  ASSERT_THROW(parser->parse(), InvalidNodeTestException);
}

TEST_F(XPathParserTest, unexpected_token_is_reported_as_an_error)
{
  tokens.add({
      XPathToken(XPathTokenType::Function, "does-not-matter"),
      XPathToken(XPathTokenType::RightParen)
  });

  ASSERT_THROW(parser->parse(), UnexpectedTokenException);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
