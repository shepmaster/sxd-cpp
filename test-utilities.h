#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <glib.h>

#include "element.h"

typedef struct {
  output_t out;
  GString *string;
} test_output_t;

void
test_output_init(test_output_t *to);

#endif
