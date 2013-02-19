#include "xpath-function-library.h"

#include "xpath-function.h"

void
XPathFunctionLibrary::add(std::shared_ptr<XPathFunction> function)
{
  _functions[function->name()] = function;
}

bool
XPathFunctionLibrary::has_function(std::string name) const
{
  return _functions.count(name) > 0;
}

std::shared_ptr<XPathFunction>
XPathFunctionLibrary::function_for_name(std::string name) const
{
  if (has_function(name)) {
    return _functions.at(name);
  }

  return nullptr;
}
