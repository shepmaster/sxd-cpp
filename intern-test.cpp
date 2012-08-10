#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "intern.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(intern)
{};

TEST(intern, new1)
{
  intern_t *i;
  i = intern_new();
  CHECK(i != NULL);
  intern_free(i);
}

TEST(intern, const_string)
{
  intern_t *i;
  const char * const input = "hello";
  const char *result;

  i = intern_new();

  result = intern_intern(i, input);
  CHECK(result != NULL);
  STRCMP_EQUAL(input, result);
  CHECK(result != input);

  intern_free(i);
}

TEST(intern, allocated_string)
{
  intern_t *i;
  char *input;
  const char *result;

  i = intern_new();
  input = strdup("hello");

  result = intern_intern(i, input);
  CHECK(result != NULL);
  STRCMP_EQUAL(input, result);
  CHECK(result != input);

  free(input);
  intern_free(i);
}

TEST(intern, two_identical_strings)
{
  intern_t *i;
  char *input1;
  char *input2;
  const char *result1;
  const char *result2;

  i = intern_new();
  input1 = strdup("hello");
  input2 = strdup("hello");

  result1 = intern_intern(i, input1);
  result2 = intern_intern(i, input2);

  CHECK(result1 != NULL);
  CHECK(result2 != NULL);

  STRCMP_EQUAL(input1, result1);
  STRCMP_EQUAL(input2, result2);
  STRCMP_EQUAL(result2, result1);
  POINTERS_EQUAL(result1, result2);

  free(input1);
  free(input2);
  intern_free(i);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
