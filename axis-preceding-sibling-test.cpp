#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"
#include "axis-preceding-sibling.h"

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

TEST(xpath_axis, axis_preceding_sibling)
{
  AxisPrecedingSibling axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.d, std::ref(visitor));

  CHECK_EQUAL(3, visitor.nodes_seen());
  CHECK_EQUAL(d.c, visitor[0]);
  CHECK_EQUAL(d.b, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
