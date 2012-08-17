#include "element.h"
#include "xpath.h"

XPath::XPath(std::string node_name) :
  _node_name(node_name)
{
}

Nodeset
XPath::select_nodes(Node &node)
{
  Nodeset result;
  auto child_selector = [&](Node *child){
    if (child->type() == NODE_TYPE_ELEMENT) {
      Element *e = dynamic_cast<Element *>(child);
      if (_node_name == e->name()) {
        result.add(e);
      }
    }
  };
  node.foreach_child(child_selector);
  return result;
}
