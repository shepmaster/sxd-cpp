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
    auto token = _source.next_token();
    auto name = token.string();
    std::unique_ptr<XPathStep> step;

    if (name == ".") {
      step = make_unique<StepSelf>();
    } else if (name == "..") {
      step = make_unique<StepParent>();
    } else {
      step = make_unique<StepChild>(name);
    }

    parts.push_back(std::move(step));
  }

  _creator(std::move(parts));
}
