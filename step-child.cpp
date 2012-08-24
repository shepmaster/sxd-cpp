#include "step-child.h"
#include "element.h"

StepChild::StepChild(std::string name) :
  _name(name)
{
}

void
StepChild::select_nodes(Node *current_node, Nodeset &step_result)
{
  auto child_selector = [&](Node *child){
    if (child->type() == NODE_TYPE_ELEMENT) {
      Element *e = dynamic_cast<Element *>(child);
      if (_name == "*" || _name == e->name()) {
        step_result.add(e);
      }
    }
  };
  current_node->foreach_child(child_selector);
}
