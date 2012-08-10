#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "nmtoken.h"

TEST_GROUP(nmtoken)
{};

TEST(nmtoken, new_nmtoken)
{
  NMToken nm("xs:boolean");

  STRCMP_EQUAL("boolean", nm.name());
  STRCMP_EQUAL("xs", nm.ns());
}

TEST(nmtoken, new_name_only)
{
  NMToken nm("string");

  STRCMP_EQUAL("string", nm.name());
  POINTERS_EQUAL(NULL, nm.ns());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
