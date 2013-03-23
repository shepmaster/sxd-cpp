#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-token-source.h"
#include "xpath-expression.h"
#include "xpath-tokenizer-buffer.h"

#include <vector>

class XPathParser {
public:
  XPathParser(XPathTokenSource &source);
  std::unique_ptr<XPathExpression> parse();

private:
  XPathTokenizerBuffer _source;
};

#endif
