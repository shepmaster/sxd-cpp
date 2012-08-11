#ifndef XPATH_COMPILED_H
#define XPATH_COMPILED_H

#include "xpath-internal.h"

class XPathCompiled {
public:
  static XPathCompiled *compile(const char * const xpath);
  ~XPathCompiled();

  void add_step(XPathStep step);
  std::vector<XPathStep> &steps();

private:
  std::vector<XPathStep> _steps;
};

#endif
