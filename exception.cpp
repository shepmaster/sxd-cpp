#include <iostream>
#include <stdlib.h>

#include "exception.h"

struct exceptionS {
  int id;
};

typedef struct {
  int id;
  const char * const name;
  int parent_id;
} exception_table_elt_t;

exception_table_elt_t exception_table[] = {
  {EX_EXCEPTION_ID, "Exception", EX_EXCEPTION_ID},
  {EX_INVALID_EXCEPTION_ID, "Invalid exception", EX_EXCEPTION_ID},
  {EX_INVALID_VALUE_ID, "Invalid value", EX_EXCEPTION_ID},
  {EX_INVALID_STRING_VALUE_ID, "Invalid string value", EX_INVALID_VALUE_ID}
};

int exception_table_size = sizeof(exception_table) / sizeof(exception_table[0]);

void
exception_free(exception_t *ex)
{
  free(ex);
}

exception_t *
exception_new(int id)
{
  exception_t *ex;
  ex = (exception_t *)calloc(1, sizeof(*ex));
  if (id >=0 && id < exception_table_size) {
    ex->id = id;
  } else {
    ex->id = EX_INVALID_EXCEPTION_ID;
  }
  return ex;
}

static int
exception_element_is_terminal(exception_table_elt_t *exx)
{
  return (exx->id == exx->parent_id);
}

int
exception_is_a(exception_t *ex, int id)
{
  exception_table_elt_t *exx;

  if (id >= exception_table_size) return FALSE;

  exx = &exception_table[ex->id];

  while (TRUE) {
    if (exx->id == id) {
      return TRUE;
    }

    if (exception_element_is_terminal(exx)) {
      return FALSE;
    }

    exx = &exception_table[exx->parent_id];
  }
}
