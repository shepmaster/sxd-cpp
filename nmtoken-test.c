#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "nmtoken.h"

static void
test_new_nmtoken(void)
{
  nmtoken_t *nm;

  nm = nmtoken_new("xs:boolean");

  assert(nm != NULL);
  assert(strcmp(nmtoken_name(nm), "boolean") == 0);
  assert(strcmp(nmtoken_namespace(nm), "xs") == 0);

  nmtoken_free(nm);
}

static void
test_new_name_only_nmtoken(void)
{
  nmtoken_t *nm;

  nm = nmtoken_new("string");

  assert(nm != NULL);
  assert(strcmp(nmtoken_name(nm), "string") == 0);
  assert(nmtoken_namespace(nm) == NULL);

  nmtoken_free(nm);
}

int
main(int argc, char **argv)
{
  test_new_nmtoken();
  test_new_name_only_nmtoken();

  return EXIT_SUCCESS;
}
