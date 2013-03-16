#ifndef EXPRESSION_RELATIONAL_H
#define EXPRESSION_RELATIONAL_H

#include "xpath-expression.h"

class ExpressionRelational : public XPathExpression {
public:
  typedef bool(*BinaryRelationalOp)(double left, double right);
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionRelational>
  LessThan(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionRelational>
  LessThanOrEqual(SubExpression left, SubExpression right);

  ExpressionRelational(SubExpression left,
                       SubExpression right,
                       BinaryRelationalOp operation);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
  BinaryRelationalOp _operation;
};

#endif
