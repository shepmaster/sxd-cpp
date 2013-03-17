#include "xpath-evaluation-context-impl.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathEvaluationContextImplTest : public ::testing::Test {
protected:
  Document doc;
  Node *node = doc.new_element("context-node");
  Nodeset nodes;
  XPathFunctionLibrary functions;
};

TEST_F(XPathEvaluationContextImplTest, has_a_context_node)
{
  XPathEvaluationContextImpl context(node, nodes, functions);

  ASSERT_EQ(node, context.node());
}

TEST_F(XPathEvaluationContextImplTest, initial_position_is_one)
{
  XPathEvaluationContextImpl context(node, nodes, functions);

  ASSERT_EQ(1, context.position());
}

TEST_F(XPathEvaluationContextImplTest, next_increments_the_position)
{
  XPathEvaluationContextImpl context(node, nodes, functions);

  context.next();

  ASSERT_EQ(2, context.position());
}

TEST_F(XPathEvaluationContextImplTest, size_is_the_count_of_nodes)
{
  auto element = doc.new_element("one");
  nodes.add(element);

  XPathEvaluationContextImpl context(node, nodes, functions);

  ASSERT_EQ(1, context.size());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
