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
  Element *e;
  const char * const name = "hello";

  doc = document_new();
  e = document_element_new(doc, name);
  CHECK(e != NULL);
  STRCMP_EQUAL(e->name(), name);
  delete e;
  document_free(doc);
}

TEST(element, mutated_name)
{
  document_t *doc;
  Element *element;
  char * name;
 
  name = strdup("hello");
  doc = document_new();
  element = document_element_new(doc, name);

  name[0] = 'y';
  STRCMP_EQUAL(element->name(), "hello");

  delete element;
  document_free(doc);
  free(name);
}

TEST(element, set_attribute)
{
  document_t *doc;
  Element *element;
  const char * const element_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  doc = document_new();
  element = document_element_new(doc, element_name);

  element->set_attribute(attr_name, attr_value);
  STRCMP_EQUAL(element->get_attribute(attr_name), attr_value);

  delete element;
  document_free(doc);
}

TEST(element, mutated_attribute)
{
  document_t *doc;
  Element *element;
  const char * const element_name = "hello";
  char *attr_name;
  char *attr_value;

  attr_name = strdup("type");
  attr_value = strdup("world");
  doc = document_new();
  element = document_element_new(doc, element_name);

  element->set_attribute(attr_name, attr_value);
  
  attr_name[0] = 'h';
  attr_value[0] = 'y';
  STRCMP_EQUAL(element->get_attribute("type"), "world");

  delete element;
  document_free(doc);
  free(attr_name);
  free(attr_value);
}

TEST(element, output_basic)
{
  document_t *doc;
  Element *element;
  StringOutput out;

  doc = document_new();
  element = document_element_new(doc, "one");

  element->output(out);
  STRCMP_EQUAL("<one />", out.string());

  delete element;
  document_free(doc);
}

TEST(element, output_attribute)
{
  document_t *doc;
  Element *element;
  StringOutput out;

  doc = document_new();
  element = document_element_new(doc, "one");
  element->set_attribute("hi", "there");

  element->output(out);
  STRCMP_EQUAL("<one hi=\"there\" />", out.string());

  delete element;
  document_free(doc);
}

TEST(element, output_child)
{
  document_t *doc;
  Element *element;
  Element *child;
  StringOutput out;

  doc = document_new();
  element = document_element_new(doc, "one");
  child = document_element_new(doc, "two");
  element->append_child(child);

  element->output(out);
  STRCMP_EQUAL("<one><two /></one>", out.string());

  delete element;
  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
