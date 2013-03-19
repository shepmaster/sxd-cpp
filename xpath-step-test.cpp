#include "xpath-step.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-axis.h"
#include "mock-xpath-node-test.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::ElementsAre;
using testing::Invoke;
using testing::Return;
using testing::Unused;
using testing::_;
using testing::NiceMock;

class XPathStepTest : public ::testing::Test {
protected:
  Document doc;
  Element *top_element = doc.new_element("top");

  shared_ptr<MockAxis> axis = make_shared<MockAxis>();
  shared_ptr<MockNodeTest> node_test = make_shared<MockNodeTest>();
  shared_ptr<MockExpression> predicate = make_shared<MockExpression>();

  Nodeset nodes;

  NiceMock<MockEvaluationContext> context;
  shared_ptr<NiceMock<MockEvaluationContext>> sub_context =
    make_shared<NiceMock<MockEvaluationContext>>();

  void SetUp() {
    EXPECT_CALL(context, node()).WillRepeatedly(Return(top_element));
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(XPathStepTest, axis_is_passed_the_node)
{
  EXPECT_CALL(*axis, select_nodes(top_element, _, nodes));

  XPathStep step(axis, node_test);

  step.select_nodes(context, nodes);
}

void
add_context_node(Node *node, Unused, Nodeset &set)
{
  set.add(node);
}

/*
TEST_F(XPathStepTest, predicate_is_evaluated)
{
  EXPECT_CALL(*axis, select_nodes(_, _, _)).WillOnce(Invoke(add_context_node));
  EXPECT_CALL(context, new_context_for(_, _)).WillOnce(Return(sub_context));
  EXPECT_CALL(*predicate, evaluate(_));

  XPathStep step(axis, node_test, predicate);

  step.select_nodes(context, nodes);
}

TEST_F(XPathStepTest, predicate_with_number_one_selects_first_node)
{
  EXPECT_CALL(*axis, select_nodes(_, _, _)).WillOnce(Invoke(add_context_node));
  EXPECT_CALL(context, new_context_for(_, _)).WillOnce(Return(sub_context));
  EXPECT_CALL(*sub_context, position()).WillOnce(Return(1));
  EXPECT_CALL(*predicate, evaluate(_)).WillOnce(Return(XPathValue(1.0)));

  XPathStep step(axis, node_test, predicate);

  step.select_nodes(context, nodes);

  ASSERT_THAT(nodes, ElementsAre(top_element));
}

TEST_F(XPathStepTest, predicate_with_string_selects_all_nodes)
{
  EXPECT_CALL(*axis, select_nodes(_, _, _)).WillOnce(Invoke(add_context_node));
  EXPECT_CALL(context, new_context_for(_, _)).WillOnce(Return(sub_context));
  EXPECT_CALL(*predicate, evaluate(_)).WillOnce(Return(XPathValue("string")));

  XPathStep step(axis, node_test, predicate);

  step.select_nodes(context, nodes);

  ASSERT_THAT(nodes, ElementsAre(top_element));
}
*/

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
