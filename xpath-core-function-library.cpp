#include "xpath-core-function-library.h"

#include "function-true.h"
#include "function-false.h"

void
XPathCoreFunctionLibrary::register_functions(XPathFunctionLibrary &library)
{
  library.add(std::make_shared<FunctionTrue>());
  library.add(std::make_shared<FunctionFalse>());
}
