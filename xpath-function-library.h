#ifndef XPATH_FUNCTION_LIBRARY_H
#define XPATH_FUNCTION_LIBRARY_H

class XPathFunction;

#include <map>

class XPathFunctionLibrary {
public:
  void add(std::shared_ptr<XPathFunction> function);
  bool has_function(std::string name) const;
  std::shared_ptr<XPathFunction> function_for_name(std::string name) const;

private:
  std::map<std::string, std::shared_ptr<XPathFunction>> _functions;
};

#endif
