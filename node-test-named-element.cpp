#include "element.h"
#include "node-test-element.h"
#include "node-test-named-element.h"

NodeTestNamedElement::NodeTestNamedElement(std::string name) :
  _name(name)
{
}

bool
NodeTestNamedElement::include_node(Node &node)
{
  NodeTestElement element_test;

  if (! element_test.include_node(node)) {
    return false;
  }

  Element *element = (Element *)&node;
  return _name.compare(element->name()) == 0;
}
