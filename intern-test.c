#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "intern.h"

static void
test_new_intern(void)
{
  intern_t *i;
  i = intern_new();
  assert(i != NULL);
  intern_free(i);
}

static void
test_intern_const_string(void)
{
  intern_t *i;
  const char * const input = "hello";
  const char *result;

  i = intern_new();

  result = intern_intern(i, input);
  assert(result != NULL);
  assert(strcmp(result, input) == 0);
  assert(result != input);

  intern_free(i);
}

static void
test_intern_allocated_string(void)
{
  intern_t *i;
  char *input;
  const char *result;

  i = intern_new();
  input = strdup("hello");

  result = intern_intern(i, input);
  assert(result != NULL);
  assert(strcmp(result, input) == 0);
  assert(result != input);

  free(input);
  intern_free(i);
}

static void
test_intern_two_identical_strings(void)
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

  assert(result1 != NULL);
  assert(result2 != NULL);

  assert(strcmp(result1, input1) == 0);
  assert(strcmp(result2, input2) == 0);
  assert(strcmp(result1, result2) == 0);
  assert(result1 == result2);

  free(input1);
  free(input2);
  intern_free(i);
}

int
main(int argc, char **argv)
{
  test_new_intern();
  test_intern_const_string();
  test_intern_allocated_string();
  test_intern_two_identical_strings();

  return EXIT_SUCCESS;
}
