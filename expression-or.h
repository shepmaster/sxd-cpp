#ifndef EXPRESSION_OR_H
#define EXPRESSION_OR_H

#include "xpath-expression.h"

class ExpressionOr : public XPathExpression {
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionOr>
  Or(SubExpression left, SubExpression right);

  ExpressionOr(SubExpression left, SubExpression right);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
};

#endif
