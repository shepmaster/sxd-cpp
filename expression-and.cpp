#include "expression-and.h"

#include "make-unique.h"

std::unique_ptr<ExpressionAnd>
ExpressionAnd::And(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionAnd>(left, right);
}

ExpressionAnd::ExpressionAnd(SubExpression left, SubExpression right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionAnd::evaluate(const XPathEvaluationContext &context) const
{
  return _left->evaluate(context).boolean() &&
    _right->evaluate(context).boolean();
}
