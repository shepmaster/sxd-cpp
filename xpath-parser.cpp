#include "xpath-parser.h"

#include "axis-child.h"
#include "axis-self.h"
#include "axis-parent.h"
#include "axis-descendant.h"
#include "axis-descendant-or-self.h"
#include "node-test-element.h"
#include "node-test-text.h"
#include "make-unique.h"

XPathParser::XPathParser(XPathTokenSource &source, const xpath_creator_fn_t &creator) :
  _source(source), _creator(creator)
{
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
parse_axis(XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathAxis> axis;

  if (name == "self") {
    axis = make_unique<AxisSelf>();
  } else if (name == "parent") {
    axis = make_unique<AxisParent>();
  } else if (name == "descendant") {
    axis = make_unique<AxisDescendant>();
  }

  source.next_token(); // Consume the colon
  return axis;
}

std::unique_ptr<XPathNodeTest>
parse_node_test(XPathTokenSource &source, XPathToken token) {
  auto name = token.string();
  std::unique_ptr<XPathNodeTest> node_test;

  if (name == "text") {
    node_test = make_unique<NodeTestText>();
  }

  source.next_token(); // Consume left paren
  source.next_token(); // Consume right paren

  return node_test;
}

void
XPathParser::parse() {
  std::vector<std::unique_ptr<XPathStep>> steps;

  while (_source.has_more_tokens()) {
    std::unique_ptr<XPathAxis> axis;
    std::unique_ptr<XPathNodeTest> node_test;
    auto token = _source.next_token();
    auto name = token.string();

    if (looks_like_axis(_source)) {
      axis = parse_axis(_source, token);
      token = _source.next_token();
      node_test = make_unique<NodeTestElement>(token.string());
    } else if (looks_like_node_test(_source)) {
      axis = make_unique<AxisChild>(); // Not true - what about if we have an axis?
      node_test = parse_node_test(_source, token);
    } else if (name == ".") {
      axis = make_unique<AxisSelf>();
      node_test = make_unique<NodeTestElement>("*");
    } else if (name == "..") {
      axis = make_unique<AxisParent>();
      node_test = make_unique<NodeTestElement>("*");
    } else if (token.is(XPathTokenType::DoubleSlash)) {
      axis = make_unique<AxisDescendantOrSelf>();
      node_test = make_unique<NodeTestElement>("*");
    } else {
      axis = make_unique<AxisChild>();
      node_test = make_unique<NodeTestElement>(name);
    }

    steps.push_back(make_unique<XPathStep>(std::move(axis), std::move(node_test)));
  }

  _creator(std::move(steps));
}
