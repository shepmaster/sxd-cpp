#ifndef XPATH_STEP_H
#define XPATH_STEP_H

#include "xpath-axis.h"
#include "xpath-node-test.h"
#include "xpath-expression.h"
#include "xpath-function-library.h"
#include "node.h"
#include "nodeset.h"

#include <memory>

class XPathStep {
public:
  XPathStep(std::shared_ptr<XPathAxis> axis,
            std::shared_ptr<XPathNodeTest> node_test,
            std::shared_ptr<XPathExpression> predicate);

  void select_nodes(const XPathEvaluationContext &context,
                    Nodeset &step_result);

private:
  std::shared_ptr<XPathAxis> _axis;
  std::shared_ptr<XPathNodeTest> _node_test;
  std::shared_ptr<XPathExpression> _predicate;

  friend std::ostream &operator<<(std::ostream &, const XPathStep &);
};

std::ostream &operator<<(std::ostream &, const XPathStep &);

#endif
