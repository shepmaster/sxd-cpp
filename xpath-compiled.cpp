#include "xpath-compiled.h"

#include "xpath-tokens.h"

XPathCompiled *
XPathCompiled::compile(const char * const xpath)
{
  XPathCompiled *compiled;
  xpath_step_t step;
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
  for (int i = 0; i < _steps.size(); i++) {
    xpath_step_t *step = &_steps[i];
    for (int j = 0; j < step->tests.size(); j++) {
      XPathNodeTest *test = step->tests[j];
      delete test;
    }
  }
}

void
XPathCompiled::add_step(xpath_step_t step)
{
  _steps.push_back(step);
}

std::vector<xpath_step_t> &
XPathCompiled::steps()
{
  return _steps;
}
