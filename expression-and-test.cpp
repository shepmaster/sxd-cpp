#include "expression-and.h"

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
using testing::_;

class ExpressionAndTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left  = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  NiceMock<MockEvaluationContext> context;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionAndTest, evaluates_left_argument)
{
  ExpressionAnd expression(left, right);

  EXPECT_CALL(*left, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionAndTest, evaluates_right_argument_when_left_is_true)
{
  ExpressionAnd expression(left, right);

  EXPECT_CALL(*left,  evaluate(Ref(context))).WillRepeatedly(Return(true));
  EXPECT_CALL(*right, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionAndTest, short_circuits_when_left_argument_is_false)
{
  ExpressionAnd expression(left, right);

  EXPECT_CALL(*left,  evaluate(Ref(context))).WillRepeatedly(Return(false));
  EXPECT_CALL(*right, evaluate(_)).Times(0);

  expression.evaluate(context);
}

struct LogicPair {
  bool left;
  bool right;
};

LogicPair params[] = {{ false,  false },
                      { false,  true  },
                      { true,   false },
                      { true,   true  }};

class ExpressionAndLogicTest : public ExpressionAndTest,
                               public ::testing::WithParamInterface<LogicPair>
{};

TEST_P(ExpressionAndLogicTest, has_and_truth_table)
{
  ExpressionAnd expression(left, right);
  auto params = GetParam();

  EXPECT_CALL(*left,  evaluate(_)).WillRepeatedly(Return(params.left));
  EXPECT_CALL(*left,  evaluate(_)).WillRepeatedly(Return(params.right));

  auto result = expression.evaluate(context);

  ASSERT_EQ(params.left && params.right, result.boolean());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
