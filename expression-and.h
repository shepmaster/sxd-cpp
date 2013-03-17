#ifndef EXPRESSION_AND_H
#define EXPRESSION_AND_H

#include "xpath-expression.h"

class ExpressionAnd : public XPathExpression {
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionAnd>
  And(SubExpression left, SubExpression right);

  ExpressionAnd(SubExpression left, SubExpression right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
};

#endif
