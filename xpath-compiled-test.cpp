#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-compiled.h"

TEST_GROUP(xpath)
{};

TEST(xpath, compile_element)
{
  const char * const name = "one";
  XPathCompiled *compiled;

  compiled = XPathCompiled::compile(name);

  std::vector<xpath_step_t> steps = compiled->steps();
  CHECK_EQUAL_C_INT(1, steps.size());
  CHECK_EQUAL(XPATH_AXIS_CHILD, steps[0].axis);
  CHECK_EQUAL(XPATH_NODE_TYPE_ELEMENT, steps[0].type);
  STRCMP_EQUAL(name, steps[0].name);

  delete compiled;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
