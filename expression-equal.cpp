#include "expression-equal.h"

#include "make-unique.h"

std::unique_ptr<ExpressionEqual>
ExpressionEqual::Equal(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionEqual>(left, right, false);
}

std::unique_ptr<ExpressionEqual>
ExpressionEqual::NotEqual(SubExpression left, SubExpression right)
{
  return make_unique<ExpressionEqual>(left, right, true);
}

ExpressionEqual::ExpressionEqual(SubExpression left,
                                 SubExpression right,
                                 bool negate) :
  _left(left), _right(right), _negate(negate)
{}

struct Equality {
  XPathValue left_val;
  XPathValue right_val;

  bool
  result() {
    if (either_is(XPathValue::Type::Boolean)) {
      return left_val.boolean() == right_val.boolean();
    } else if (either_is(XPathValue::Type::Number)) {
      return left_val.number() == right_val.number();
    } else {
      return left_val.string() == right_val.string();
    }
  }

  bool
  either_is(XPathValue::Type type) {
    return left_val.is(type) || right_val.is(type);
  }
};

XPathValue
ExpressionEqual::evaluate(const XPathEvaluationContext &context) const
{
  Equality equality = {_left->evaluate(context), _right->evaluate(context)};

  bool result = equality.result();

  if (_negate) return !result;
  else         return result;
}
