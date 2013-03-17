#include "expression-math.h"

#include "make-unique.h"
#include <cmath>

double add(double left, double right) { return left + right; }
double subtract(double left, double right) { return left - right; }
double multiply(double left, double right) { return left * right; }
double divide(double left, double right) { return left / right; }

std::unique_ptr<ExpressionMath>
ExpressionMath::Addition(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionMath>(left, right, add);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Subtraction(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionMath>(left, right, subtract);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Multiplication(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionMath>(left, right, multiply);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Division(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionMath>(left, right, divide);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Remainder(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionMath>(left, right, fmod);
}

ExpressionMath::ExpressionMath(SubExpression left,
                               SubExpression right,
                               BinaryMathOp operation) :
  _left(left), _right(right), _operation(operation)
{}

XPathValue
ExpressionMath::evaluate(const XPathEvaluationContext &context) const
{
  auto left = _left->evaluate(context);
  auto right = _right->evaluate(context);
  return XPathValue(_operation(left.number(), right.number()));
}
