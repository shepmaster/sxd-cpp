#ifndef XPATH_PARSER_H
#define XPATH_PARSER_H

#include "xpath-step.h"
#include "xpath-token-source.h"

#include <functional>
#include <vector>

class XPathParser {
public:
  typedef std::function<void (std::vector<std::unique_ptr<XPathStep>> &&)> xpath_creator_fn_t;

  XPathParser(XPathTokenSource &source, const xpath_creator_fn_t &creator);
  void parse();

private:
  XPathTokenSource &_source;
  xpath_creator_fn_t _creator;
};

#endif
