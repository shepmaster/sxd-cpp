#include "expression-or.h"

ExpressionOr::ExpressionOr(std::shared_ptr<XPathExpression> left,
                           std::shared_ptr<XPathExpression> right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionOr::evaluate(const XPathEvaluationContext &context) const
{
  return _left->evaluate(context).boolean() ||
    _right->evaluate(context).boolean();
}
