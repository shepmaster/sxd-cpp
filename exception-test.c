#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "exception.h"

#define T_EX_NONEXISTENT_ID 99999

static void
test_new_exception(void)
{
  exception_t *e;
  e = exception_new(EX_EXCEPTION_ID);
  assert(e != NULL);
  assert(exception_is_a(e, EX_EXCEPTION_ID));
  exception_free(e);
}

static void
test_invalid_value_exception(void)
{
  exception_t *e;
  e = exception_new(EX_INVALID_VALUE_ID);
  assert(e != NULL);
  assert(exception_is_a(e, EX_EXCEPTION_ID));
  assert(exception_is_a(e, EX_INVALID_VALUE_ID));
  assert(! exception_is_a(e, EX_INVALID_STRING_VALUE_ID));
  exception_free(e);
}

static void
test_invalid_string_value_exception(void)
{
  exception_t *e;
  e = exception_new(EX_INVALID_STRING_VALUE_ID);
  assert(e != NULL);
  assert(exception_is_a(e, EX_EXCEPTION_ID));
  assert(exception_is_a(e, EX_INVALID_VALUE_ID));
  assert(exception_is_a(e, EX_INVALID_STRING_VALUE_ID));
  exception_free(e);
}

static void
test_nonexistent_exception(void)
{
  exception_t *e;
  e = exception_new(T_EX_NONEXISTENT_ID);
  assert(e != NULL);
  assert(exception_is_a(e, EX_INVALID_EXCEPTION_ID));
  exception_free(e);
}

static void
test_is_a_nonexistent_exception(void)
{
  exception_t *e;
  e = exception_new(EX_EXCEPTION_ID);
  assert(e != NULL);
  assert(! exception_is_a(e, T_EX_NONEXISTENT_ID));
  exception_free(e);
}

int
main(int argc, char **argv)
{
  test_new_exception();
  test_invalid_value_exception();
  test_invalid_string_value_exception();
  test_nonexistent_exception();
  test_is_a_nonexistent_exception();

  return EXIT_SUCCESS;
}
