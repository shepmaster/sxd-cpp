#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-step.h"
#include "xpath-token-source.h"

#include <functional>
#include <vector>

class XPathParser {
public:
  XPathParser(XPathTokenSource &source);
  std::unique_ptr<XPathExpression> parse();

private:
  XPathTokenSource &_source;
};

#endif
