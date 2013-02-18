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
#include "make-unique.h"

using std::move;

XPathParser::XPathParser(XPathTokenSource &source, XPathCreator &creator) :
  _source(source), _creator(creator)
{
}

void
consume(XPathTokenSource &source, XPathTokenType type) {
  source.next_token();
}

bool
looks_like_axis(XPathTokenSource &_source) {
  return _source.next_token_is(XPathTokenType::DoubleColon);
}

bool
looks_like_node_test(XPathTokenSource &_source) {
  // Should we also look for the closing paren?
  return _source.next_token_is(XPathTokenType::LeftParen);
}

std::unique_ptr<XPathAxis>
parse_axis(XPathCreator &creator, XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathAxis> axis;

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
    creator.invalid_axis(name);
    return nullptr;
  }

  consume(source, XPathTokenType::DoubleColon);
  return axis;
}

std::unique_ptr<XPathNodeTest>
parse_node_test(XPathCreator &creator, XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathNodeTest> node_test;

  if (name == "node") {
    node_test = make_unique<NodeTestNode>();
  } else if (name == "text") {
    node_test = make_unique<NodeTestText>();
  } else {
    creator.invalid_node_test(name);
    return nullptr;
  }

  consume(source, XPathTokenType::LeftParen);
  consume(source, XPathTokenType::RightParen);

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
    consume(source, XPathTokenType::LeftParen);
    consume(source, XPathTokenType::RightParen);
    return make_unique<ExpressionFunction>(token.string());
  } else {
    return nullptr;
  }
}

void
XPathParser::parse() {
  while (_source.has_more_tokens()) {
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
    } else if (token.is(XPathTokenType::DoubleSlash)) {
      axis = make_unique<AxisDescendantOrSelf>();
      node_test = make_unique<NodeTestNode>();
    } else {
      if (looks_like_axis(_source)) {
        axis = parse_axis(_creator, _source, token);
        token = _source.next_token();
        name = token.string();
      } else if (token.is(XPathTokenType::AtSign)) {
        axis = make_unique<AxisAttribute>();
        token = _source.next_token();
        name = token.string();
      } else {
        axis = make_unique<AxisChild>();
      }

      if (! axis) {
        return;
      }

      if (looks_like_node_test(_source)) {
        node_test = parse_node_test(_creator, _source, token);
      } else {
        node_test = default_node_test(axis, token);
      }

      if (! node_test) {
        return;
      }
    }

    std::unique_ptr<XPathExpression> predicate;
    if (_source.next_token_is(XPathTokenType::LeftBracket)) {
      consume(_source, XPathTokenType::LeftBracket);
      predicate = parse_primary_expression(_source);
      consume(_source, XPathTokenType::RightBracket);
    }

    _creator.add_step(make_unique<XPathStep>(move(axis), move(node_test), move(predicate)));
  }
}
