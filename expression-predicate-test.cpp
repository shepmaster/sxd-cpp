#include "expression-predicate.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::ElementsAre;
using testing::Return;
using testing::_;
using testing::NiceMock;

class ExpressionStepTest : public ::testing::Test {
protected:
  Document doc;

  shared_ptr<MockExpression> node_expr = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> predicate_expr = make_shared<NiceMock<MockExpression>>();

  Nodeset nodes;

  NiceMock<MockEvaluationContext> context;
  shared_ptr<NiceMock<MockEvaluationContext>> sub_context =
    make_shared<NiceMock<MockEvaluationContext>>();

  void SetUp() {
    EXPECT_CALL(context, new_context_for(_, _)).WillRepeatedly(Return(sub_context));

    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionStepTest, selecting_expression_is_evaluated)
{
  EXPECT_CALL(*node_expr, evaluate(Ref(context)));

  ExpressionPredicate step(node_expr, predicate_expr);

  step.evaluate(context);
}

TEST_F(ExpressionStepTest, predicate_expression_is_evaluated_for_each_input)
{
  auto input_node = doc.new_element("node1");

  nodes.add(input_node);

  EXPECT_CALL(*node_expr, evaluate(_)).WillRepeatedly(Return(nodes));
  EXPECT_CALL(*predicate_expr, evaluate(Ref(*sub_context)));

  ExpressionPredicate step(node_expr, predicate_expr);

  step.evaluate(context);
}

TEST_F(ExpressionStepTest, numeric_predicate_selects_that_node)
{
  auto input_node_1 = doc.new_element("node1");
  auto input_node_2 = doc.new_element("node2");

  nodes.add(input_node_1);
  nodes.add(input_node_2);

  EXPECT_CALL(*node_expr, evaluate(_)).WillRepeatedly(Return(nodes));
  EXPECT_CALL(*sub_context, position())
    .WillOnce(Return(1))
    .WillOnce(Return(2));
  EXPECT_CALL(*predicate_expr, evaluate(_)).WillRepeatedly(Return(XPathValue(1.0)));

  ExpressionPredicate step(node_expr, predicate_expr);

  auto value = step.evaluate(context);

  ASSERT_THAT(value.nodeset(), ElementsAre(input_node_1));
}

TEST_F(ExpressionStepTest, false_predicate_selects_no_nodes)
{
  auto input_node_1 = doc.new_element("node1");
  auto input_node_2 = doc.new_element("node2");

  nodes.add(input_node_1);
  nodes.add(input_node_2);

  EXPECT_CALL(*node_expr, evaluate(_)).WillRepeatedly(Return(nodes));
  EXPECT_CALL(*predicate_expr, evaluate(_)).WillRepeatedly(Return(XPathValue(false)));

  ExpressionPredicate step(node_expr, predicate_expr);

  auto value = step.evaluate(context);

  ASSERT_THAT(value.nodeset(), ElementsAre());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
