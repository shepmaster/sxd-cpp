#include "expression-or.h"

#include "make-unique.h"

std::unique_ptr<ExpressionOr>
ExpressionOr::Or(SubExpression left, SubExpression right) {
  return make_unique<ExpressionOr>(left, right);
}

ExpressionOr::ExpressionOr(SubExpression left, SubExpression right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionOr::evaluate(const XPathEvaluationContext &context) const
{
  return _left->evaluate(context).boolean() ||
    _right->evaluate(context).boolean();
}
