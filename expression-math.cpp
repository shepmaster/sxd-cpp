#include "expression-math.h"

#include "make-unique.h"
#include <cmath>

double add(double left, double right) { return left + right; }
double subtract(double left, double right) { return left - right; }
double multiply(double left, double right) { return left * right; }
double divide(double left, double right) { return left / right; }

std::unique_ptr<ExpressionMath>
ExpressionMath::Addition(std::shared_ptr<XPathExpression> left,
                         std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, add);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Subtraction(std::shared_ptr<XPathExpression> left,
                            std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, subtract);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Multiplication(std::shared_ptr<XPathExpression> left,
                               std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, multiply);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Division(std::shared_ptr<XPathExpression> left,
                         std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, divide);
}

std::unique_ptr<ExpressionMath>
ExpressionMath::Remainder(std::shared_ptr<XPathExpression> left,
                          std::shared_ptr<XPathExpression> right)
{
  return make_unique<ExpressionMath>(left, right, fmod);
}

ExpressionMath::ExpressionMath(std::shared_ptr<XPathExpression> left,
                               std::shared_ptr<XPathExpression> right,
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
