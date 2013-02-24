#ifndef EXPRESSION_MATH_H
#define EXPRESSION_MATH_H

#include "xpath-expression.h"

class ExpressionMath : public XPathExpression {
public:
  typedef double (*BinaryMathOp)(double left, double right);

  static std::unique_ptr<ExpressionMath>
  Addition(std::shared_ptr<XPathExpression> left,
           std::shared_ptr<XPathExpression> right);

  static std::unique_ptr<ExpressionMath>
  Subtraction(std::shared_ptr<XPathExpression> left,
              std::shared_ptr<XPathExpression> right);

  ExpressionMath(std::shared_ptr<XPathExpression> left,
                 std::shared_ptr<XPathExpression> right,
                 BinaryMathOp operation);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  std::shared_ptr<XPathExpression> _left;
  std::shared_ptr<XPathExpression> _right;
  BinaryMathOp _operation;
};

#endif
