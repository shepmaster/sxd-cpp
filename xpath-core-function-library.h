#ifndef XPATH_CORE_FUNCTION_LIBRARY_H
#define XPATH_CORE_FUNCTION_LIBRARY_H

#include "xpath-function-library.h"

class XPathCoreFunctionLibrary {
public:
  static void register_functions(XPathFunctionLibrary &library);
};

#endif
