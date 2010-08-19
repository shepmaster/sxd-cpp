#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test-utilities.h"

#include "document.h"
#include "text-node.h"

static void
test_new_text_node(void)
{
  document_t *doc;
  text_node_t *tn;

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");
  assert(strcmp(text_node_text(tn), "I am text") == 0);
  text_node_free(tn);
  document_free(doc);
}

static void
test_output(void)
{
  document_t *doc;
  text_node_t *tn;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");

  text_node_output(tn, &to.out);
  assert(strcmp("I am text", to.string->str) == 0);

  text_node_free(tn);
  document_free(doc);
  test_output_destroy(&to);
}

int
main(int argc, char **argv)
{
  test_new_text_node();
  test_output();

  return EXIT_SUCCESS;
}
