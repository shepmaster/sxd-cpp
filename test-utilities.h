#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <glib.h>

#include "element.h"

Node *
test_helper_new_node(document_t *doc, const char * const name);

Node *
test_helper_new_text_node(document_t *doc, const char * const text);

typedef struct {
  output_t out;
  GString *string;
} test_output_t;

void
test_output_init(test_output_t *to);

void
test_output_destroy(test_output_t *to);

nodeset_t *
nodeset_new_with_nodes(Node *first, ...);

#endif
