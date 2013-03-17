#ifndef EXPRESSION_MATH_H
#define EXPRESSION_MATH_H

#include "xpath-expression.h"

class ExpressionMath : public XPathExpression {
public:
  typedef double (*BinaryMathOp)(double left, double right);
  typedef std::shared_ptr<XPathExpression> SubExpression;

  static std::unique_ptr<ExpressionMath>
  Addition(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionMath>
  Subtraction(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionMath>
  Multiplication(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionMath>
  Division(SubExpression left, SubExpression right);

  static std::unique_ptr<ExpressionMath>
  Remainder(SubExpression left, SubExpression right);

  ExpressionMath(SubExpression left,
                 SubExpression right,
                 BinaryMathOp operation);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _left;
  SubExpression _right;
  BinaryMathOp _operation;
};

#endif
