#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-axis.h"

/**
 * Selects children Node%s.
 */
class AxisChild : public XPathAxis {
public:
  void select_nodes(XPathEvaluationContext const & context,
                    XPathNodeTest const &node_test,
                    Nodeset &result);
  std::ostream &to_stream(std::ostream &strm) const;
};

#endif
