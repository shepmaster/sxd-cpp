#include "expression-math.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::ValuesIn;
using testing::_;

class ExpressionMathTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  NiceMock<MockEvaluationContext> context;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

static double math_no_op(double left, double right) { return 0; }

TEST_F(ExpressionMathTest, evaluates_both_arguments)
{
  ExpressionMath expression(left, right, math_no_op);

  EXPECT_CALL(*left, evaluate(Ref(context)));
  EXPECT_CALL(*right, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionMathTest, adds_arguments)
{
  auto expression = ExpressionMath::Addition(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.0));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(1.0, result.number());
}

TEST_F(ExpressionMathTest, subtracts_arguments)
{
  auto expression = ExpressionMath::Subtraction(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.0));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(-5.0, result.number());
}

TEST_F(ExpressionMathTest, multiplies_arguments)
{
  auto expression = ExpressionMath::Multiplication(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.1));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.1));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(-6.51, result.number());
}

TEST_F(ExpressionMathTest, divides_arguments)
{
  auto expression = ExpressionMath::Division(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-0.5));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(2.0));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(-0.25, result.number());
}

TEST_F(ExpressionMathTest, calculates_remainder_of_division_of_arguments)
{
  auto expression = ExpressionMath::Remainder(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(1.5));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(0.4));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(0.3, result.number());
}

struct ModuloParams {
  double left;
  double right;
  double result;
};

// These examples taken directly from the spec
ModuloParams params[] = {{ 5,  2,  1 },
                         { 5, -2,  1 },
                         {-5,  2, -1 },
                         {-5, -2, -1 }};

class ExpressionMathRemainderTest : public ExpressionMathTest,
                                    public ::testing::WithParamInterface<ModuloParams>
{};

TEST_P(ExpressionMathRemainderTest, remainder_truncates)
{
  auto expression = ExpressionMath::Remainder(left, right);
  auto params = GetParam();

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(params.left));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(params.right));

  auto result = expression->evaluate(context);
  ASSERT_DOUBLE_EQ(params.result, result.number());
}

INSTANTIATE_TEST_CASE_P(TruncatingRemainder,
                        ExpressionMathRemainderTest,
                        ValuesIn(params));

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
