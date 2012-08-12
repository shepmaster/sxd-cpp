#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-descendant.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_descendant)
{
  AxisDescendant axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.alpha, std::ref(visitor));

  CHECK_EQUAL(10, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
  CHECK_EQUAL(d.a, visitor[1]);
  CHECK_EQUAL(d.b, visitor[2]);
  CHECK_EQUAL(d.c, visitor[3]);
  CHECK_EQUAL(d.d, visitor[4]);
  CHECK_EQUAL(d.two, visitor[5]);
  CHECK_EQUAL(d.w, visitor[6]);
  CHECK_EQUAL(d.x, visitor[7]);
  CHECK_EQUAL(d.y, visitor[8]);
  CHECK_EQUAL(d.z, visitor[9]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
