#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-preceding.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_preceding)
{
  AxisPreceding axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.x, std::ref(visitor));

  CHECK_EQUAL(6, visitor.nodes_seen());
  CHECK_EQUAL(d.w, visitor[0]);
  CHECK_EQUAL(d.one, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
  CHECK_EQUAL(d.b, visitor[3]);
  CHECK_EQUAL(d.c, visitor[4]);
  CHECK_EQUAL(d.d, visitor[5]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
