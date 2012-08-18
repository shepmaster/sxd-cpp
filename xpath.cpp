#include "element.h"
#include "xpath.h"

XPath::XPath(std::vector<std::string> node_names) :
  _node_names(node_names)
{
}

Nodeset
XPath::select_nodes(Node &node)
{
  Nodeset result;
  result.add(&node);

  for (auto name : _node_names) {
    Nodeset step_result;
    auto child_selector = [&](Node *child){
      if (child->type() == NODE_TYPE_ELEMENT) {
        Element *e = dynamic_cast<Element *>(child);
        if (name == "*" || name == e->name()) {
          step_result.add(e);
        }
      }
    };
    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];
      current_node->foreach_child(child_selector);
    }
    result = step_result;
  }
  return result;
}
