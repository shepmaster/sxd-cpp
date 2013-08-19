#ifndef AXIS_DESCENDANT_OR_SELF_H
#define AXIS_DESCENDANT_OR_SELF_H

#include "xpath-axis.h"

/**
 * Selects this Node and all child Nodes in document order.
 */
class AxisDescendantOrSelf : public XPathAxis {
public:
  void select_nodes(XPathEvaluationContext const & context,
                    XPathNodeTest const &node_test,
                    Nodeset &result);
  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
