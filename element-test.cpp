#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"

#include "document.h"
#include "node.h"
#include "element.h"

TEST_GROUP(element)
{
};

TEST(element, new_element)
{
  document_t *doc;
  element_t *e;
  const char * const name = "hello";

  doc = document_new();
  e = document_element_new(doc, name);
  CHECK(e != NULL);
  STRCMP_EQUAL(element_name(e), name);
  element_free(e);
  document_free(doc);
}

TEST(element, cast_to_node)
{
  document_t *doc;
  element_t *e;
  node_t *n;
  const char * const name = "hello";

  doc = document_new();
  e = document_element_new(doc, name);

  n = element_cast_to_node(e);
  POINTERS_EQUAL(e, n);
  CHECK(n != NULL);
  POINTERS_EQUAL(n->document(), doc);

  element_free(e);
  document_free(doc);
}

TEST(element, mutated_name)
{
  document_t *doc;
  element_t *element;
  char * name;
 
  name = strdup("hello");
  doc = document_new();
  element = document_element_new(doc, name);

  name[0] = 'y';
  STRCMP_EQUAL(element_name(element), "hello");

  element_free(element);
  document_free(doc);
  free(name);
}

TEST(element, set_attribute)
{
  document_t *doc;
  element_t *element;
  const char * const element_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  doc = document_new();
  element = document_element_new(doc, element_name);

  element_set_attribute(element, attr_name, attr_value);
  STRCMP_EQUAL(element_get_attribute(element, attr_name), attr_value);

  element_free(element);
  document_free(doc);
}

TEST(element, mutated_attribute)
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
  STRCMP_EQUAL(element_get_attribute(element, "type"), "world");

  element_free(element);
  document_free(doc);
  free(attr_name);
  free(attr_value);
}

TEST(element, output_basic)
{
  document_t *doc;
  element_t *element;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");

  element->output(&to.out);
  STRCMP_EQUAL("<one />", to.string->str);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

TEST(element, output_attribute)
{
  document_t *doc;
  element_t *element;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");
  element_set_attribute(element, "hi", "there");

  element->output(&to.out);
  STRCMP_EQUAL("<one hi=\"there\" />", to.string->str);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

TEST(element, output_child)
{
  document_t *doc;
  element_t *element;
  element_t *child;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  element = document_element_new(doc, "one");
  child = document_element_new(doc, "two");
  element->append_child(child);

  element->output(&to.out);
  STRCMP_EQUAL("<one><two /></one>", to.string->str);

  element_free(element);
  document_free(doc);
  test_output_destroy(&to);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
