#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-step.h"
#include "xpath-token-source.h"

#include <functional>
#include <vector>

class XPathParseErrorNotifier {
public:
  virtual void invalid_axis(std::string axis) = 0;
  virtual void invalid_node_test(std::string name) = 0;
};

class XPathParser {
public:
  XPathParser(XPathTokenSource &source, XPathParseErrorNotifier &creator);
  std::unique_ptr<XPathExpression> parse();

private:
  XPathTokenSource &_source;
  XPathParseErrorNotifier &_error_notifier;
};

#endif
