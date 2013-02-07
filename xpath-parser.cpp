#include "xpath-parser.h"

#include "axis-child.h"
#include "axis-self.h"
#include "axis-parent.h"
#include "axis-any-child.h"
#include "node-test-element.h"
#include "make-unique.h"

XPathParser::XPathParser(XPathTokenSource &source, const xpath_creator_fn_t &creator) :
  _source(source), _creator(creator)
{
}

bool
looks_like_axis(XPathTokenSource &_source) {
  return _source.next_token_is(XPathTokenType::DoubleColon);
}

void
parse_axis(XPathTokenSource &_source, std::function<void(std::unique_ptr<NodeTestElement> &&)> make_axis)
{
  auto token = _source.next_token();

  if (token.is(XPathTokenType::DoubleColon)) {
    token = _source.next_token();
    make_axis(make_unique<NodeTestElement>(token.string()));
  }
}

void
XPathParser::parse() {
  std::vector<std::unique_ptr<XPathStep>> steps;

  while (_source.has_more_tokens()) {
    std::unique_ptr<XPathAxis> axis;
    auto token = _source.next_token();
    auto name = token.string();

    if (token.is(XPathTokenType::DoubleSlash)) {
      // Hmm, what can follow this?
      // Does foo//child::bar make sense?
      // Assume string for now
      token = _source.next_token();
      axis = make_unique<AxisAnyChild>(make_unique<NodeTestElement>(token.string()));
    } else if (name == "self" && looks_like_axis(_source)) {
      parse_axis(_source, [&](std::unique_ptr<NodeTestElement> &&test) {
          axis = make_unique<AxisSelf>(std::move(test));
        });
    } else if (name == "parent" && looks_like_axis(_source)) {
      parse_axis(_source, [&](std::unique_ptr<NodeTestElement> &&test) {
          axis = make_unique<AxisParent>(std::move(test));
        });
    } else if (name == ".") {
      axis = make_unique<AxisSelf>(make_unique<NodeTestElement>("*"));
    } else if (name == "..") {
      axis = make_unique<AxisParent>(make_unique<NodeTestElement>("*"));
    } else {
      axis = make_unique<AxisChild>(make_unique<NodeTestElement>(name));
    }

    steps.push_back(make_unique<XPathStep>(std::move(axis)));
  }

  _creator(std::move(steps));
}
