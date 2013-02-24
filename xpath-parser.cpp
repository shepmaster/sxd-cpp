#include "xpath-parser.h"

#include "axis-child.h"
#include "axis-self.h"
#include "axis-parent.h"
#include "axis-descendant.h"
#include "axis-descendant-or-self.h"
#include "axis-attribute.h"
#include "node-test-node.h"
#include "node-test-element.h"
#include "node-test-attribute.h"
#include "node-test-text.h"
#include "expression-literal.h"
#include "expression-function.h"
#include "expression-path.h"
#include "expression-math.h"
#include "expression-negation.h"
#include "make-unique.h"
#include "xpath-parsing-exceptions.h"

using std::move;

XPathParser::XPathParser(XPathTokenSource &source) :
  _source(source)
{
}

void
consume(XPathTokenSource &source, XPathTokenType type) {
  source.next_token();
}

std::unique_ptr<XPathAxis>
parse_axis(XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathAxis> axis;

  if (source.next_token_is(XPathTokenType::DoubleColon)) {
    if (name == "self") {
      axis = make_unique<AxisSelf>();
    } else if (name == "parent") {
      axis = make_unique<AxisParent>();
    } else if (name == "descendant") {
      axis = make_unique<AxisDescendant>();
    } else if (name == "descendant-or-self") {
      axis = make_unique<AxisDescendantOrSelf>();
    } else if (name == "attribute") {
      axis = make_unique<AxisAttribute>();
    } else {
      throw InvalidXPathAxisException(name);
    }

    consume(source, XPathTokenType::DoubleColon);
  }

  return axis;
}

std::unique_ptr<XPathNodeTest>
parse_node_test(XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathNodeTest> node_test;

  if (source.next_token_is(XPathTokenType::LeftParen)) {
    if (name == "node") {
      node_test = make_unique<NodeTestNode>();
    } else if (name == "text") {
      node_test = make_unique<NodeTestText>();
    } else {
      throw InvalidNodeTestException(name);
    }

    consume(source, XPathTokenType::LeftParen);
    consume(source, XPathTokenType::RightParen);
  }

  return node_test;
}

std::unique_ptr<XPathNodeTest>
default_node_test(std::unique_ptr<XPathAxis> &axis, XPathToken token) {
  switch (axis->principal_node_type()) {
  case PrincipalNodeType::Attribute:
    return make_unique<NodeTestAttribute>(token.string());
  case PrincipalNodeType::Element:
    return make_unique<NodeTestElement>(token.string());
  }
}

std::unique_ptr<XPathExpression>
parse_string_literal(XPathTokenSource &source) {
  if (source.next_token_is(XPathTokenType::Apostrophe)) {
    consume(source, XPathTokenType::Apostrophe);
    auto token = source.next_token();
    consume(source, XPathTokenType::Apostrophe);
    return make_unique<ExpressionLiteral>(token.string());
  } else if (source.next_token_is(XPathTokenType::DoubleQuote)) {
    consume(source, XPathTokenType::DoubleQuote);
    auto token = source.next_token();
    consume(source, XPathTokenType::DoubleQuote);
    return make_unique<ExpressionLiteral>(token.string());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_numeric_literal(XPathTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Number)) {
    auto token = source.next_token();
    return make_unique<ExpressionLiteral>(token.number());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathTokenSource &source);

std::unique_ptr<XPathExpression>
parse_function_call(XPathTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::String)) {
    std::vector<std::shared_ptr<XPathExpression>> arguments;

    auto token = source.next_token();
    consume(source, XPathTokenType::LeftParen);
    while (! source.next_token_is(XPathTokenType::RightParen)) {
      // TODO: this should be the top-level expression
      arguments.push_back(parse_primary_expression(source));
    }
    consume(source, XPathTokenType::RightParen);

    return make_unique<ExpressionFunction>(token.string(), std::move(arguments));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathTokenSource &source) {
  std::unique_ptr<XPathExpression> expr;

  expr = parse_string_literal(source);
  if (expr) return expr;

  expr = parse_numeric_literal(source);
  if (expr) return expr;

  expr = parse_function_call(source);
  if (expr) return expr;

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_path_expression(XPathTokenSource &_source)
{
  if (! (_source.next_token_is(XPathTokenType::String) ||
         _source.next_token_is(XPathTokenType::ParentNode))) {
    return nullptr;
  }

  std::vector<std::unique_ptr<XPathStep>> steps;

  while(true) {
    std::unique_ptr<XPathAxis> axis;
    std::unique_ptr<XPathNodeTest> node_test;
    auto token = _source.next_token();
    auto name = token.string();

    if (token.is(XPathTokenType::ParentNode)) {
      axis = make_unique<AxisParent>();
      node_test = make_unique<NodeTestNode>();
    } else {
      axis = parse_axis(_source, token);
      if (axis) {
        token = _source.next_token();
        name = token.string();
      } else {
        axis = make_unique<AxisChild>();
      }

      node_test = parse_node_test(_source, token);
      if (! node_test) {
        node_test = default_node_test(axis, token);
      }
    }

    std::unique_ptr<XPathExpression> predicate;
    if (_source.next_token_is(XPathTokenType::LeftBracket)) {
      consume(_source, XPathTokenType::LeftBracket);
      predicate = parse_primary_expression(_source);
      consume(_source, XPathTokenType::RightBracket);
    }

    steps.push_back(make_unique<XPathStep>(move(axis), move(node_test), move(predicate)));

    if (_source.next_token_is(XPathTokenType::Slash)) {
      consume(_source, XPathTokenType::Slash);
    } else {
      break;
    }
  }

  return make_unique<ExpressionPath>(move(steps));
}

std::unique_ptr<XPathExpression>
parse_unary_expression(XPathTokenSource &source)
{
  auto expr = parse_primary_expression(source);
  if (expr) return expr;

  if (source.next_token_is(XPathTokenType::MinusSign)) {
    consume(source, XPathTokenType::MinusSign);
    expr = parse_unary_expression(source);
    return make_unique<ExpressionNegation>(move(expr));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_multiplicative_expression(XPathTokenSource &source)
{
  auto left = parse_unary_expression(source);
  if (!left) return nullptr;

  while (source.has_more_tokens()) {
    if (source.next_token_is(XPathTokenType::Multiply)) {
      consume(source, XPathTokenType::Multiply);
      auto right = parse_unary_expression(source);
      left = ExpressionMath::Multiplication(move(left), move(right));
    } else if (source.next_token_is(XPathTokenType::Divide)) {
      consume(source, XPathTokenType::Divide);
      auto right = parse_unary_expression(source);
      left = ExpressionMath::Division(move(left), move(right));
    } else if (source.next_token_is(XPathTokenType::Remainder)) {
      consume(source, XPathTokenType::Remainder);
      auto right = parse_unary_expression(source);
      left = ExpressionMath::Remainder(move(left), move(right));
    } else {
      break;
    }
  }

  return left;
}

std::unique_ptr<XPathExpression>
parse_additive_expression(XPathTokenSource &source)
{
  auto left = parse_multiplicative_expression(source);
  if (!left) return nullptr;

  while (source.has_more_tokens()) {
    if (source.next_token_is(XPathTokenType::PlusSign)) {
      consume(source, XPathTokenType::PlusSign);
      auto right = parse_multiplicative_expression(source);
      left = ExpressionMath::Addition(move(left), move(right));
    } else if (source.next_token_is(XPathTokenType::MinusSign)) {
      consume(source, XPathTokenType::MinusSign);
      auto right = parse_multiplicative_expression(source);
      left = ExpressionMath::Subtraction(move(left), move(right));
    } else {
      break;
    }
  }

  return left;
}

std::unique_ptr<XPathExpression>
XPathParser::parse() {
  std::unique_ptr<XPathExpression> expr;

  expr = parse_path_expression(_source);
  if (expr) return expr;

  expr = parse_additive_expression(_source);
  if (expr) return expr;

  return nullptr;
}
