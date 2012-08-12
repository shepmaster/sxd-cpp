#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-parent.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_parent)
{
  AxisParent axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(1, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
