#ifndef MOCK_XPATH_AXIS_H
#define MOCK_XPATH_AXIS_H

#include "xpath-axis.h"

class MockAxis : public XPathAxis {
public:
  MOCK_METHOD3(select_nodes, void(XPathEvaluationContext const &context,
                                  XPathNodeTest const &test,
                                  Nodeset &result));
};

#endif
