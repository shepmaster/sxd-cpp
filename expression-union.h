#ifndef EXPRESSION_UNION_H
#define EXPRESSION_UNION_H

#include "xpath-expression.h"

class ExpressionUnion : public XPathExpression {
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionUnion>
  Union(SubExpression left, SubExpression right);

  ExpressionUnion(SubExpression left, SubExpression right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
};

#endif
