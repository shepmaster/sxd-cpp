#include "expression-equal.h"

ExpressionEqual::ExpressionEqual(std::shared_ptr<XPathExpression> left,
                                 std::shared_ptr<XPathExpression> right) :
  _left(left), _right(right)
{}

static bool
either_is(XPathValue left, XPathValue right, XPathValue::Type type)
{
  return left.is(type) || right.is(type);
}

XPathValue
ExpressionEqual::evaluate(const XPathEvaluationContext &context) const
{
  auto left_val = _left->evaluate(context);
  auto right_val = _right->evaluate(context);

  if (either_is(left_val, right_val, XPathValue::Type::Boolean)) {
    return left_val.boolean() == right_val.boolean();
  } else if (either_is(left_val, right_val, XPathValue::Type::Number)) {
    return left_val.number() == right_val.number();
  } else {
    return left_val.string() == right_val.string();
  }
}
