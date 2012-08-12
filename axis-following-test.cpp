#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-following.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_following)
{
  AxisFollowing axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.c, std::ref(visitor));

  CHECK_EQUAL(6, visitor.nodes_seen());
  CHECK_EQUAL(d.d, visitor[0]);
  CHECK_EQUAL(d.two, visitor[1]);
  CHECK_EQUAL(d.w, visitor[2]);
  CHECK_EQUAL(d.x, visitor[3]);
  CHECK_EQUAL(d.y, visitor[4]);
  CHECK_EQUAL(d.z, visitor[5]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
