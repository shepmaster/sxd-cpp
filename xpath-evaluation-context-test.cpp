#include "xpath-evaluation-context.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathEvaluationContextTest : public ::testing::Test {
protected:
  Nodeset nodes;
};

TEST_F(XPathEvaluationContextTest, initial_position_is_one)
{
  XPathEvaluationContext context(nodes);

  ASSERT_EQ(1, context.position());
}

TEST_F(XPathEvaluationContextTest, next_increments_the_position)
{
  XPathEvaluationContext context(nodes);

  context.next();

  ASSERT_EQ(2, context.position());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
