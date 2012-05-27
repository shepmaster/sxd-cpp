#include <stdio.h>
#include <stdlib.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "exception.h"

#define T_EX_NONEXISTENT_ID 99999

TEST_GROUP(exception)
{};

TEST(exception, new_exception)
{
  exception_t *e;
  e = exception_new(EX_EXCEPTION_ID);
  CHECK(e != NULL);
  CHECK(exception_is_a(e, EX_EXCEPTION_ID));
  exception_free(e);
}

TEST(exception, invalid_value_exception)
{
  exception_t *e;
  e = exception_new(EX_INVALID_VALUE_ID);
  CHECK(e != NULL);
  CHECK(exception_is_a(e, EX_EXCEPTION_ID));
  CHECK(exception_is_a(e, EX_INVALID_VALUE_ID));
  CHECK(! exception_is_a(e, EX_INVALID_STRING_VALUE_ID));
  exception_free(e);
}

TEST(exception, invalid_string_value_exception)
{
  exception_t *e;
  e = exception_new(EX_INVALID_STRING_VALUE_ID);
  CHECK(e != NULL);
  CHECK(exception_is_a(e, EX_EXCEPTION_ID));
  CHECK(exception_is_a(e, EX_INVALID_VALUE_ID));
  CHECK(exception_is_a(e, EX_INVALID_STRING_VALUE_ID));
  exception_free(e);
}

TEST(exception, nonexistent_exception)
{
  exception_t *e;
  e = exception_new(T_EX_NONEXISTENT_ID);
  CHECK(e != NULL);
  CHECK(exception_is_a(e, EX_INVALID_EXCEPTION_ID));
  exception_free(e);
}

TEST(exception, is_a_nonexistent_exception)
{
  exception_t *e;
  e = exception_new(EX_EXCEPTION_ID);
  CHECK(e != NULL);
  CHECK(! exception_is_a(e, T_EX_NONEXISTENT_ID));
  exception_free(e);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
