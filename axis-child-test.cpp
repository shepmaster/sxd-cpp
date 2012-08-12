#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "remembering-node-visitor.h"

#include "axis-child.h"

TEST_GROUP(AxisChild)
{
  XPathAxisTestData d;
};

TEST(AxisChild, visits_children)
{
  AxisChild axis;
  RememberingNodeVisitor visitor;

  axis.traverse(d.alpha, std::ref(visitor));

  CHECK_EQUAL(2, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
  CHECK_EQUAL(d.two, visitor[1]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
