#include "expression-relational.h"

#include "make-unique.h"

std::unique_ptr<ExpressionRelational>
ExpressionRelational::LessThan(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionRelational>(left, right);
}

ExpressionRelational::ExpressionRelational(SubExpression left, SubExpression right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionRelational::evaluate(const XPathEvaluationContext &context) const
{
  auto left_val = _left->evaluate(context);
  auto right_val = _right->evaluate(context);
  return left_val.number() < right_val.number();
}
