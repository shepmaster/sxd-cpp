#ifndef EXPRESSION_RELATIONAL_H
#define EXPRESSION_RELATIONAL_H

#include "xpath-expression.h"

class ExpressionRelational : public XPathExpression {
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionRelational>
  LessThan(SubExpression left, SubExpression right);

  ExpressionRelational(SubExpression left, SubExpression right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
};

#endif
