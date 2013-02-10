#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-step.h"
#include "xpath-token-source.h"

#include <functional>
#include <vector>

class XPathCreator {
public:
  virtual void add_step(std::unique_ptr<XPathStep> step) = 0;
};

class XPathParser {
public:
  XPathParser(XPathTokenSource &source, XPathCreator &creator);
  void parse();

private:
  XPathTokenSource &_source;
  XPathCreator &_creator;
};

#endif
