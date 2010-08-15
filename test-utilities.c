#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "test-utilities.h"

static void
test_output_fn(void *string_as_void, const char *format, ...)
{
  GString *string = string_as_void;
  va_list params;

  va_start(params, format);
  g_string_append_vprintf(string, format, params);
  va_end(params);
}

void
test_output_init(test_output_t *to)
{
  to->string = g_string_new(NULL);
  to->out.fn = test_output_fn;
  to->out.data = to->string;
}
