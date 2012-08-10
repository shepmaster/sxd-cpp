#include <iostream>
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
  Document doc;
  Element *e;
  const char * const name = "hello";

  e = doc.new_element(name);
  CHECK(e != NULL);
  STRCMP_EQUAL(e->name(), name);
  delete e;
}

TEST(element, mutated_name)
{
  Document doc;
  Element *element;
  char * name;
 
  name = strdup("hello");
  element = doc.new_element(name);

  name[0] = 'y';
  STRCMP_EQUAL(element->name(), "hello");

  delete element;
  free(name);
}

TEST(element, set_attribute)
{
  Document doc;
  Element *element;
  const char * const element_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  element = doc.new_element(element_name);

  element->set_attribute(attr_name, attr_value);
  STRCMP_EQUAL(element->get_attribute(attr_name), attr_value);

  delete element;
}

TEST(element, mutated_attribute)
{
  Document doc;
  Element *element;
  const char * const element_name = "hello";
  char *attr_name;
  char *attr_value;

  attr_name = strdup("type");
  attr_value = strdup("world");
  element = doc.new_element(element_name);

  element->set_attribute(attr_name, attr_value);
  
  attr_name[0] = 'h';
  attr_value[0] = 'y';
  STRCMP_EQUAL(element->get_attribute("type"), "world");

  delete element;
  free(attr_name);
  free(attr_value);
}

TEST(element, output_basic)
{
  Document doc;
  Element *element;
  StringOutput out;

  element = doc.new_element("one");

  element->output(out);
  STRCMP_EQUAL("<one />", out.string());

  delete element;
}

TEST(element, output_attribute)
{
  Document doc;
  Element *element;
  StringOutput out;

  element = doc.new_element("one");
  element->set_attribute("hi", "there");

  element->output(out);
  STRCMP_EQUAL("<one hi=\"there\" />", out.string());

  delete element;
}

TEST(element, output_child)
{
  Document doc;
  Element *element;
  Element *child;
  StringOutput out;

  element = doc.new_element("one");
  child = doc.new_element("two");
  element->append_child(child);

  element->output(out);
  STRCMP_EQUAL("<one><two /></one>", out.string());

  delete element;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
