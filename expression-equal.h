#ifndef EXPRESSION_EQUAL_H
#define EXPRESSION_EQUAL_H

#include "xpath-expression.h"

class ExpressionEqual : public XPathExpression {
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionEqual>
  Equal(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionEqual>
  NotEqual(SubExpression left, SubExpression right);

  ExpressionEqual(SubExpression left, SubExpression right, bool negate);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
  bool _negate;
};

#endif
