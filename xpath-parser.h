#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-token-source.h"
#include "xpath-expression.h"
#include "xpath-parser-token-source.h"

#include <vector>

class XPathParser {
public:
  XPathParser(XPathTokenSource &source);
  std::unique_ptr<XPathExpression> parse();

private:
  XPathParserTokenSource _source;
};

#endif
