#include "xpath-parser.h"

#include "step-child.h"
#include "step-self.h"
#include "step-parent.h"
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
    if (token.is(XPathTokenType::DoubleColon)) {
      _source.next_token(); // Eat the next string
      step = make_unique<StepSelf>();
    } else {
      auto name = token.string();

      if (name == ".") {
        step = make_unique<StepSelf>();
      } else if (name == "..") {
        step = make_unique<StepParent>();
      } else {
        step = make_unique<StepChild>(name);
      }
    }

    parts.push_back(std::move(step));
  }

  _creator(std::move(parts));
}
