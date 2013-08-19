#ifndef AXIS_SELF_H
#define AXIS_SELF_H

#include "xpath-axis.h"

/**
 * Selects the current Node.
 */
class AxisSelf : public XPathAxis {
public:
  void select_nodes(XPathEvaluationContext const & context,
                    XPathNodeTest const &node_test,
                    Nodeset &result);
  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
