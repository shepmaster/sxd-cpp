#include "xpath-function-library.h"

void
XPathFunctionLibrary::add(std::shared_ptr<XPathFunction> function)
{
  _functions[function->name()] = function;
}

std::shared_ptr<XPathFunction>
XPathFunctionLibrary::function_for_name(std::string name) const
{
  if (_functions.count(name) > 0) {
    return _functions.at(name);
  }

  return nullptr;
}
