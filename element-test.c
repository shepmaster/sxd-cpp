#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test-utilities.h"

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
test_output_basic(void)
{
  document_t *doc;
  element_t *element;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");

  element_output(element, &to.out);
  assert(strcmp("<one />", to.string->str) == 0);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

static void
test_output_attribute(void)
{
  document_t *doc;
  element_t *element;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");
  element_set_attribute(element, "hi", "there");

  element_output(element, &to.out);
  assert(strcmp("<one hi=\"there\" />", to.string->str) == 0);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

static void
test_output_child(void)
{
  document_t *doc;
  element_t *element;
  element_t *child;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");
  child = document_element_new(doc, "two");
  node_append_child(element_cast_to_node(element), element_cast_to_node(child));

  element_output(element, &to.out);
  assert(strcmp("<one><two /></one>", to.string->str) == 0);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

int
main(int argc, char **argv)
{
  test_new_element();
  test_element_cast_to_node();
  test_mutated_name();
  test_set_attribute();
  test_mutated_attribute();
  test_output_basic();
  test_output_attribute();
  test_output_child();

  return EXIT_SUCCESS;
}
