#include "axis-attribute.h"
#include "element.h"

void
AxisAttribute::select_nodes(XPathEvaluationContext const &context,
                            XPathNodeTest const &node_test,
                            Nodeset &result) {
  if (context.node()->type() == NODE_TYPE_ELEMENT) {
    Element *element = dynamic_cast<Element *>(context.node());

    auto child_selector = [&](Node *attr) {
      auto attr_context = context.new_context_for(1);
      attr_context->next(attr);

      node_test.test(*attr_context, result);
    };
    element->foreach_attribute(child_selector);
  }
}

PrincipalNodeType
AxisAttribute::principal_node_type() {
  return PrincipalNodeType::Attribute;
};

std::ostream &
AxisAttribute::to_stream(std::ostream &strm) const
{
  return strm << "attribute::";
};
