#include "expression-or.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "expression-test-support.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionOrTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left  = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  ExpressionTestSupport support;
  XPathEvaluationContext context = support.context();

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionOrTest, evaluates_left_argument)
{
  ExpressionOr expression(left, right);

  EXPECT_CALL(*left, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionOrTest, evaluates_right_argument_when_left_is_false)
{
  ExpressionOr expression(left, right);

  EXPECT_CALL(*left,  evaluate(Ref(context))).WillRepeatedly(Return(false));
  EXPECT_CALL(*right, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionOrTest, short_circuits_when_left_argument_is_true)
{
  ExpressionOr expression(left, right);

  EXPECT_CALL(*left,  evaluate(Ref(context))).WillRepeatedly(Return(true));
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

class ExpressionOrLogicTest : public ExpressionOrTest,
                              public ::testing::WithParamInterface<LogicPair>
{};

TEST_P(ExpressionOrLogicTest, has_or_truth_table)
{
  ExpressionOr expression(left, right);
  auto params = GetParam();

  EXPECT_CALL(*left,  evaluate(_)).WillRepeatedly(Return(params.left));
  EXPECT_CALL(*left,  evaluate(_)).WillRepeatedly(Return(params.right));

  auto result = expression.evaluate(context);

  ASSERT_EQ(params.left || params.right, result.boolean());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
