#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-following-sibling.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_following_sibling)
{
  AxisFollowingSibling axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(2, visitor.nodes_seen());
  CHECK_EQUAL(d.c, visitor[0]);
  CHECK_EQUAL(d.d, visitor[1]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
