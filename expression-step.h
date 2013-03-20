#ifndef EXPRESSION_STEP_H
#define EXPRESSION_STEP_H

#include "xpath-axis.h"
#include "xpath-node-test.h"
#include "xpath-expression.h"

#include <memory>

class ExpressionStep : public XPathExpression {
public:
  ExpressionStep(std::shared_ptr<XPathAxis> axis,
                 std::shared_ptr<XPathNodeTest> node_test);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

  std::ostream &to_stream(std::ostream &strm) const;

private:
  std::shared_ptr<XPathAxis> _axis;
  std::shared_ptr<XPathNodeTest> _node_test;
};

#endif
