#include "xpath-compiled.h"

#include "xpath-tokens.h"

XPathCompiled *
XPathCompiled::compile(const char * const xpath)
{
  XPathCompiled *compiled;
  XPathStep step;
  int i;

  compiled = new XPathCompiled;

  XPathTokens tokens(xpath);
  for (i = 0; i < tokens.size(); i++) {
    xpath_token_t token = tokens[i];
    switch (token.type) {
    case TEXT:
      step.axis = XPATH_AXIS_CHILD;
      step.tests.push_back(new NamedElementTest(tokens.string(i)));
      compiled->add_step(step);
      break;
    default:
      break;
    }
  }

  return compiled;
}

XPathCompiled::~XPathCompiled()
{
  for (auto step : _steps) {
    for (auto test : step.tests) {
      delete test;
    }
  }
}

void
XPathCompiled::add_step(XPathStep step)
{
  _steps.push_back(step);
}

std::vector<XPathStep> &
XPathCompiled::steps()
{
  return _steps;
}
