#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "nmtoken.h"
}

TEST_GROUP(nmtoken)
{};

TEST(nmtoken, new_nmtoken)
{
  nmtoken_t *nm;

  nm = nmtoken_new("xs:boolean");

  CHECK(nm != NULL);
  STRCMP_EQUAL("boolean", nmtoken_name(nm));
  STRCMP_EQUAL("xs", nmtoken_namespace(nm));

  nmtoken_free(nm);
}

TEST(nmtoken, new_name_only)
{
  nmtoken_t *nm;

  nm = nmtoken_new("string");

  CHECK(nm != NULL);
  STRCMP_EQUAL( "string", nmtoken_name(nm));
  POINTERS_EQUAL(NULL, nmtoken_namespace(nm));

  nmtoken_free(nm);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
