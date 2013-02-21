#include "xpath-evaluation-context.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathEvaluationContextTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset nodes;
  XPathFunctionLibrary functions;
};

TEST_F(XPathEvaluationContextTest, initial_position_is_one)
{
  XPathEvaluationContext context(nodes, functions);

  ASSERT_EQ(1, context.position());
}

TEST_F(XPathEvaluationContextTest, next_increments_the_position)
{
  XPathEvaluationContext context(nodes, functions);

  context.next();

  ASSERT_EQ(2, context.position());
}

TEST_F(XPathEvaluationContextTest, size_is_the_count_of_nodes)
{
  auto element = doc.new_element("one");
  nodes.add(element);

  XPathEvaluationContext context(nodes, functions);

  ASSERT_EQ(1, context.size());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
