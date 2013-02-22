#include "xpath-core-function-library.h"

#include "function-last.h"
#include "function-not.h"
#include "function-true.h"
#include "function-false.h"

void
XPathCoreFunctionLibrary::register_functions(XPathFunctionLibrary &library)
{
  library.add(std::make_shared<FunctionLast>());
  library.add(std::make_shared<FunctionNot>());
  library.add(std::make_shared<FunctionTrue>());
  library.add(std::make_shared<FunctionFalse>());
}
