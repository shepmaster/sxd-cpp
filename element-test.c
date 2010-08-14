#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"
#include "node.h"
#include "element.h"

static void
test_new_element(void)
{
  document_t *doc;
  element_t *e;
  const char * const name = "hello";

  doc = document_new();
  e = document_element_new(doc, name);
  assert(e != NULL);
  assert(strcmp(element_name(e), name) == 0);
  element_free(e);
}

static void
test_element_cast_to_node(void)
{
  document_t *doc;
  element_t *e;
  node_t *n;
  const char * const name = "hello";

  doc = document_new();
  e = document_element_new(doc, name);

  n = element_cast_to_node(e);
  assert((void *)e == (void *)n);
  assert(n != NULL);
  assert(node_document(n) == doc);

  element_free(e);
}

static void
test_mutated_name(void)
{
  document_t *doc;
  element_t *element;
  char * name;
 
  name = strdup("hello");
  doc = document_new();
  element = document_element_new(doc, name);

  name[0] = 'y';
  assert(strcmp(element_name(element), "hello") == 0);

  element_free(element);
  document_free(doc);
  free(name);
}

static void
test_set_attribute(void)
{
  document_t *doc;
  element_t *element;
  const char * const element_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  doc = document_new();
  element = document_element_new(doc, element_name);

  element_set_attribute(element, attr_name, attr_value);
  assert(strcmp(element_get_attribute(element, attr_name), attr_value) == 0);

  element_free(element);
  document_free(doc);
}

static void
test_mutated_attribute(void)
{
  document_t *doc;
  element_t *element;
  const char * const element_name = "hello";
  char *attr_name;
  char *attr_value;

  attr_name = strdup("type");
  attr_value = strdup("world");
  doc = document_new();
  element = document_element_new(doc, element_name);

  element_set_attribute(element, attr_name, attr_value);
  
  attr_name[0] = 'h';
  attr_value[0] = 'y';
  assert(strcmp(element_get_attribute(element, "type"), "world") == 0);

  element_free(element);
  document_free(doc);
  free(attr_name);
  free(attr_value);
}

static void
test_output(void)
{
  document_t *doc;
  element_t *element;

  doc = document_new();
  element = document_element_new(doc, "one");

  element_output(element);

  element_free(element);
  document_free(doc);
}

int
main(int argc, char **argv)
{
  test_new_element();
  test_element_cast_to_node();
  test_mutated_name();
  test_set_attribute();
  test_mutated_attribute();
  test_output();

  return EXIT_SUCCESS;
}
