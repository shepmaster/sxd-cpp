#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-descendant-or-self.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_descendant_or_self)
{
  AxisDescendantOrSelf axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.alpha, std::ref(visitor));

  CHECK_EQUAL(11, visitor.nodes_seen());
  CHECK_EQUAL(d.alpha, visitor[0]);
  CHECK_EQUAL(d.one, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
  CHECK_EQUAL(d.b, visitor[3]);
  CHECK_EQUAL(d.c, visitor[4]);
  CHECK_EQUAL(d.d, visitor[5]);
  CHECK_EQUAL(d.two, visitor[6]);
  CHECK_EQUAL(d.w, visitor[7]);
  CHECK_EQUAL(d.x, visitor[8]);
  CHECK_EQUAL(d.y, visitor[9]);
  CHECK_EQUAL(d.z, visitor[10]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
