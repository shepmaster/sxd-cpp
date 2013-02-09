#include "axis-descendant-or-self.h"

AxisDescendantOrSelf::AxisDescendantOrSelf(std::unique_ptr<XPathNodeTest> &&node_test) :
  _node_test(std::move(node_test))
{
}

void
AxisDescendantOrSelf::select_nodes(Node *current_node, Nodeset &result)
{
  auto child_selector = [&](Node *child){
    // Which order is correct?
    _node_test->test(child, result);
    this->select_nodes(child, result);
  };
  current_node->foreach_child(child_selector);
}

std::ostream &
AxisDescendantOrSelf::to_string(std::ostream &strm) const
{
  return strm << "descendant-or-self::";
};
