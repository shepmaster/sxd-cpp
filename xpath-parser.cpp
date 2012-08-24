#include "xpath-parser.h"

#include "step-child.h"
#include "step-self.h"
#include "step-parent.h"
#include "node-test-element.h"
#include "make-unique.h"

XPathParser::XPathParser(XPathTokenSource &source, const xpath_creator_fn_t &creator) :
  _source(source), _creator(creator)
{
}

void
XPathParser::parse() {
  std::vector<std::unique_ptr<XPathStep>> parts;

  while (_source.has_more_tokens()) {
    std::unique_ptr<XPathStep> step;
    auto token = _source.next_token();
    auto name = token.string();

    if (name == "self") {
      token = _source.next_token(); // Check if this is an axis specifier

      if (token.is(XPathTokenType::DoubleColon)) {
        token = _source.next_token();
        step = make_unique<StepSelf>(make_unique<NodeTestElement>(token.string()));
      }

      // Not handling the case where the node is called "self"!
    } else if (name == ".") {
      step = make_unique<StepSelf>(make_unique<NodeTestElement>("*"));
    } else if (name == "..") {
      step = make_unique<StepParent>();
    } else {
      step = make_unique<StepChild>(make_unique<NodeTestElement>(name));
    }

    parts.push_back(std::move(step));
  }

  _creator(std::move(parts));
}
