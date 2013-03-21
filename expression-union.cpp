#include "expression-union.h"

#include "make-unique.h"

std::unique_ptr<ExpressionUnion>
ExpressionUnion::Union(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionUnion>(left, right);
}

ExpressionUnion::ExpressionUnion(SubExpression left, SubExpression right) :
  _left(left), _right(right)
{}

XPathValue
ExpressionUnion::evaluate(const XPathEvaluationContext &context) const
{
  auto left_val = _left->evaluate(context).nodeset();
  auto right_val = _right->evaluate(context).nodeset();
  left_val.add_nodeset(right_val);

  return left_val;
}
