#include "xpath-evaluation-context-impl.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-variable-bindings.h"

#include <iostream>

using testing::NiceMock;
using testing::DefaultValue;

class XPathEvaluationContextImplTest : public ::testing::Test {
protected:
  Document doc;
  Node *node = doc.new_element("context-node");
  XPathFunctionLibrary functions;
  NiceMock<MockVariableBindings> variables;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(XPathEvaluationContextImplTest, has_a_context_node)
{
  XPathEvaluationContextImpl context(0, functions, variables);
  context.next(node);

  ASSERT_EQ(node, context.node());
}

TEST_F(XPathEvaluationContextImplTest, initial_position_is_one)
{
  XPathEvaluationContextImpl context(0, functions, variables);
  context.next(node);

  ASSERT_EQ(1, context.position());
}

TEST_F(XPathEvaluationContextImplTest, next_increments_the_position)
{
  XPathEvaluationContextImpl context(0, functions, variables);
  context.next(node);
  context.next(node);

  ASSERT_EQ(2, context.position());
}

TEST_F(XPathEvaluationContextImplTest, has_a_size)
{
  XPathEvaluationContextImpl context(33, functions, variables);

  ASSERT_EQ(33, context.size());
}

TEST_F(XPathEvaluationContextImplTest, delegates_existence_to_the_variable_bindings)
{
  EXPECT_CALL(variables, value_for("foo"));

  XPathEvaluationContextImpl context(0, functions, variables);

  context.variable_for_name("foo");
}

TEST_F(XPathEvaluationContextImplTest, delegates_getter_to_the_variable_bindings)
{
  EXPECT_CALL(variables, has_value("foo"));

  XPathEvaluationContextImpl context(0, functions, variables);

  context.has_variable("foo");
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
