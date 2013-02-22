#include "xpath.h"
#include "xpath-core-function-library.h"

XPath::XPath()
{
}

XPath::XPath(std::vector<std::unique_ptr<XPathStep>> &&steps) :
  _steps(std::move(steps))
{
}

Nodeset
XPath::select_nodes(Node *node)
{
  Nodeset result;
  result.add(node);

  Nodeset junk;
  XPathFunctionLibrary functions;
  XPathCoreFunctionLibrary::register_functions(functions);

  for (auto &step : _steps) {
    Nodeset step_result;

    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];

      XPathEvaluationContext context(current_node, junk, functions);

      step->select_nodes(context, step_result);
    }

    result = step_result;
  }

  return result;
}
