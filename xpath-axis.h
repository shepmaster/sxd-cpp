#ifndef XPATH_AXIS_H
#define XPATH_AXIS_H

#include "to-stream.h"
#include "node.h"
#include "xpath-node-test.h"
#include "nodeset.h"

enum class PrincipalNodeType {
  Attribute,
  Element
};

/**
 * A directed traversal of Node%s.
 */
class XPathAxis : public ToStream {
public:
  /**
   * Applies the given node test to the nodes selected by this axis,
   * adding matching nodes to the nodeset.
   */
  virtual void
  select_nodes(Node *current_node, XPathNodeTest const & node_test, Nodeset &result) = 0;

  /**
   * Describes what node type is naturally selected by this axis.
   */
  virtual PrincipalNodeType principal_node_type() {
    return PrincipalNodeType::Element;
  };
};

#endif
