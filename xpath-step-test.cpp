#include "xpath-step.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::ElementsAre;
using testing::Invoke;
using testing::Return;
using testing::Unused;
using testing::_;

class MockAxis : public XPathAxis {
public:
  MOCK_METHOD3(select_nodes, void(Node *node, XPathNodeTest const &test, Nodeset &result));
};

class MockNodeTest : public XPathNodeTest {
public:
  MOCK_CONST_METHOD2(test, void(Node *node, Nodeset &result));
};

class MockExpression : public XPathExpression {
public:
  MOCK_CONST_METHOD1(evaluate, XPathValue(const XPathEvaluationContext &context));
};

class XPathStepTest : public ::testing::Test {
protected:
  Document doc;
  Element *top_element;

  shared_ptr<MockAxis> axis = make_shared<MockAxis>();
  shared_ptr<MockNodeTest> node_test = make_shared<MockNodeTest>();
  shared_ptr<MockExpression> predicate = make_shared<MockExpression>();
  Nodeset nodes;

  void SetUp() {
    top_element = doc.new_element("top");

    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }

  void TearDown() {
    delete top_element;
  }
};

TEST_F(XPathStepTest, axis_is_passed_the_node)
{
  EXPECT_CALL(*axis, select_nodes(top_element, _, nodes));

  XPathStep step(axis, node_test, nullptr);

  step.select_nodes(top_element, nodes);
}

void
add_context_node(Node *node, Unused, Nodeset &set)
{
  set.add(node);
}

TEST_F(XPathStepTest, predicate_is_evaluated)
{
  EXPECT_CALL(*axis, select_nodes(_, _, _)).WillOnce(Invoke(add_context_node));
  EXPECT_CALL(*predicate, evaluate(_));

  XPathStep step(axis, node_test, predicate);

  step.select_nodes(top_element, nodes);
}

TEST_F(XPathStepTest, predicate_with_number_one_selects_first_node)
{
  EXPECT_CALL(*axis, select_nodes(_, _, _)).WillOnce(Invoke(add_context_node));
  EXPECT_CALL(*predicate, evaluate(_)).WillOnce(Return(XPathValue(1.0)));

  XPathStep step(axis, node_test, predicate);

  step.select_nodes(top_element, nodes);

  ASSERT_THAT(nodes, ElementsAre(top_element));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
