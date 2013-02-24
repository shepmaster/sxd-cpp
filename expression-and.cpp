#include "expression-and.h"

ExpressionAnd::ExpressionAnd(std::shared_ptr<XPathExpression> left,
                             std::shared_ptr<XPathExpression> right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionAnd::evaluate(const XPathEvaluationContext &context) const
{
  return _left->evaluate(context).boolean() &&
    _right->evaluate(context).boolean();
}
