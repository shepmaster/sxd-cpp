#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

/* test_text_node_append_to_element(void) */
/* { */
/*   document_t *doc; */
/*   node_t *node; */
/*   text_node_t *tn; */

/*   doc = document_new(); */
/*   node = document_new_node(doc);   */
/*   tn = text_node_new("I am text"); */

/*   node_append_node((node_t *)tn); */

/*   text_node_free(tn); */
/* } */

int
main(int argc, char **argv)
{
  test_new_text_node();

  return EXIT_SUCCESS;
}
