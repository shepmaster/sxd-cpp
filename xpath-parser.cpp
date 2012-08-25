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
parse_axis(XPathTokenSource &_source, std::function<void(std::unique_ptr<NodeTestElement> &&)> make_step)
{
  auto token = _source.next_token();

  if (token.is(XPathTokenType::DoubleColon)) {
    token = _source.next_token();
    make_step(make_unique<NodeTestElement>(token.string()));
  }
}

void
XPathParser::parse() {
  std::vector<std::unique_ptr<XPathStep>> parts;

  while (_source.has_more_tokens()) {
    std::unique_ptr<XPathStep> step;
    auto token = _source.next_token();
    auto name = token.string();

    if (name == "self") {
      parse_axis(_source, [&](std::unique_ptr<NodeTestElement> &&test) {
          step = make_unique<StepSelf>(std::move(test));
        });

      // Not handling the case where the node is called "self"!
    } else if (name == "parent") {
      parse_axis(_source, [&](std::unique_ptr<NodeTestElement> &&test) {
          step = make_unique<StepParent>(std::move(test));
        });

      // Not handling the case where the node is called "parent"!
    } else if (name == ".") {
      step = make_unique<StepSelf>(make_unique<NodeTestElement>("*"));
    } else if (name == "..") {
      step = make_unique<StepParent>(make_unique<NodeTestElement>("*"));
    } else {
      step = make_unique<StepChild>(make_unique<NodeTestElement>(name));
    }

    parts.push_back(std::move(step));
  }

  _creator(std::move(parts));
}
