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
parse_primary_expression(XPathTokenSource &source) {
  auto token = source.next_token();

  if (token.is(XPathTokenType::Apostrophe)) {
    token = source.next_token();
    consume(source, XPathTokenType::Apostrophe);
    return make_unique<ExpressionLiteral>(token.string());
  } else if (token.is(XPathTokenType::DoubleQuote)) {
    token = source.next_token();
    consume(source, XPathTokenType::DoubleQuote);
    return make_unique<ExpressionLiteral>(token.string());
  } else if (token.is(XPathTokenType::Number)) {
    return make_unique<ExpressionLiteral>(token.number());
  } else if (token.is(XPathTokenType::String)) {
    std::vector<std::shared_ptr<XPathExpression>> arguments;
    consume(source, XPathTokenType::LeftParen);
    while (! source.next_token_is(XPathTokenType::RightParen)) {
      // TODO: this should be the top-level expression
      arguments.push_back(parse_primary_expression(source));
    }
    consume(source, XPathTokenType::RightParen);
    return make_unique<ExpressionFunction>(token.string(), std::move(arguments));
  } else {
    return nullptr;
  }
}

bool
looks_like_path(XPathTokenSource &source)
{
  return source.next_token_is(XPathTokenType::String) ||
    source.next_token_is(XPathTokenType::DoubleSlash) ||
    source.next_token_is(XPathTokenType::AtSign);
}

std::unique_ptr<XPathExpression>
parse_path_expression(XPathTokenSource &_source)
{
  if (! _source.next_token_is(XPathTokenType::String)) return nullptr;

  std::vector<std::unique_ptr<XPathStep>> steps;

  while(true) {
    std::unique_ptr<XPathAxis> axis;
    std::unique_ptr<XPathNodeTest> node_test;
    auto token = _source.next_token();
    auto name = token.string();

    if (name == ".") {
      axis = make_unique<AxisSelf>();
      node_test = make_unique<NodeTestNode>();
    } else if (name == "..") {
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
XPathParser::parse() {
  std::unique_ptr<XPathExpression> expr;

  expr = parse_path_expression(_source);
  if (expr) return expr;

  expr = parse_primary_expression(_source);
  if (expr) {
    if (_source.has_more_tokens()) {
      if (_source.next_token_is(XPathTokenType::PlusSign)) {
        consume(_source, XPathTokenType::PlusSign);
        auto expr2 = parse();
        return make_unique<ExpressionAddition>(move(expr), move(expr2));
      }
    }
    return expr;
  }

  return nullptr;
}
