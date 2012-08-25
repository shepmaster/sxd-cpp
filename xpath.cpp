#include "xpath.h"

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

  for (auto &step : _steps) {
    Nodeset step_result;

    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];
      step->select_nodes(current_node, step_result);
    }

    result = step_result;
  }

  return result;
}
