#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "element.h"
#include "xpath-compiled.h"

TEST_GROUP(xpath)
{};

TEST(xpath, compile_element)
{
  Document doc;
  Element *matching_element = doc.new_element("one");
  Element *non_matching_element = doc.new_element("two");

  const char * const name = "one";
  XPathCompiled *compiled;

  compiled = XPathCompiled::compile(name);

  auto steps = compiled->steps();
  CHECK_EQUAL_C_INT(1, steps.size());
  CHECK_EQUAL(XPATH_AXIS_CHILD, steps[0].axis);
  CHECK_EQUAL_C_INT(1, steps[0].tests.size());

  XPathNodeTest *test = steps[0].tests[0];
  CHECK(test->include_node(*matching_element));
  CHECK(! test->include_node(*non_matching_element));

  delete compiled;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
