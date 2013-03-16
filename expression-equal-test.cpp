#include "expression-equal.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-value-impl.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionEqualTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left  = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  shared_ptr<MockValueImpl> left_val = make_shared<NiceMock<MockValueImpl>>();
  shared_ptr<MockValueImpl> right_val = make_shared<NiceMock<MockValueImpl>>();

  Nodeset nodes;
  XPathFunctionLibrary functions;
  shared_ptr<XPathEvaluationContext> context;

  void SetUp() {
    context = make_shared<XPathEvaluationContext>(nullptr, nodes, functions);
    DefaultValue<XPathValue>::Set(XPathValue(0.0));

    setup_sub_expression(left, left_val);
    setup_sub_expression(right, right_val);
  }

  void setup_sub_expression(shared_ptr<MockExpression> &expr,
                            shared_ptr<MockValueImpl> &value) {
    EXPECT_CALL(*expr, evaluate(_)).WillRepeatedly(Return(XPathValue(value)));
    EXPECT_CALL(*value, is(_)).WillRepeatedly(Return(false));
  }
};

TEST_F(ExpressionEqualTest, evaluates_both_arguments)
{
  auto expression = ExpressionEqual::Equal(left, right);

  EXPECT_CALL(*left, evaluate(Ref(*context)));
  EXPECT_CALL(*right, evaluate(Ref(*context)));

  expression->evaluate(*context);
}

TEST_F(ExpressionEqualTest, compares_as_boolean_if_one_argument_is_a_boolean)
{
  auto expression = ExpressionEqual::Equal(left, right);

  EXPECT_CALL(*left_val, is(XPathValue::Type::Boolean)).WillRepeatedly(Return(true));

  EXPECT_CALL(*left_val, boolean());
  EXPECT_CALL(*right_val, boolean());

  expression->evaluate(*context);
}

TEST_F(ExpressionEqualTest, compares_as_number_if_one_argument_is_a_number)
{
  auto expression = ExpressionEqual::Equal(left, right);

  EXPECT_CALL(*right_val, is(XPathValue::Type::Number)).WillRepeatedly(Return(true));

  EXPECT_CALL(*left_val, number());
  EXPECT_CALL(*right_val, number());

  expression->evaluate(*context);
}

TEST_F(ExpressionEqualTest, compares_as_string_otherwise)
{
  auto expression = ExpressionEqual::Equal(left, right);

  EXPECT_CALL(*left_val, string());
  EXPECT_CALL(*right_val, string());

  expression->evaluate(*context);
}

TEST_F(ExpressionEqualTest, not_equal_returns_the_negation)
{
  auto expression = ExpressionEqual::NotEqual(left, right);

  EXPECT_CALL(*left_val, string()).WillRepeatedly(Return("yes"));
  EXPECT_CALL(*right_val, string()).WillRepeatedly(Return("no"));

  auto result = expression->evaluate(*context);
  ASSERT_EQ(XPathValue(true), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
