#ifndef XPATH_COMPILED_H
#define XPATH_COMPILED_H

#include "xpath-internal.h"

class XPathCompiled {
public:
  static XPathCompiled *compile(const char * const xpath);
  ~XPathCompiled();

  void add_step(xpath_step_t step);
  std::vector<xpath_step_t> &steps();

private:
  std::vector<xpath_step_t> _steps;
};

#endif
