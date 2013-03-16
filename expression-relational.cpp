#include "expression-relational.h"

#include "make-unique.h"

bool less_than(double left, double right) { return left < right; }
bool less_than_or_equal(double left, double right) { return left <= right; }

std::unique_ptr<ExpressionRelational>
ExpressionRelational::LessThan(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionRelational>(left, right, less_than);
}

std::unique_ptr<ExpressionRelational>
ExpressionRelational::LessThanOrEqual(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionRelational>(left, right, less_than_or_equal);
}

ExpressionRelational::ExpressionRelational(SubExpression left,
                                           SubExpression right,
                                           BinaryRelationalOp operation) :
  _left(left), _right(right), _operation(operation)
{}

XPathValue
ExpressionRelational::evaluate(const XPathEvaluationContext &context) const
{
  auto left_val = _left->evaluate(context);
  auto right_val = _right->evaluate(context);
  return _operation(left_val.number(), right_val.number());
}
