#include "element.h"

#include "document.h"
#include "test-utilities.h"

#include "gmock/gmock.h"
#include <iostream>

class ElementTest : public ::testing::Test {
protected:
  Document doc;
};

TEST_F(ElementTest, new_element)
{
  Element *element;
  const char * const name = "hello";

  element = doc.new_element(name);
  ASSERT_TRUE(element != NULL);
  ASSERT_EQ(element->name(), name);
}

TEST_F(ElementTest, element_can_have_a_namespace)
{
  auto element = doc.new_element("test:namespace", "element");
  auto qname = element->qname();

  ASSERT_EQ(QName("test:namespace", "element"), qname);
}

TEST_F(ElementTest, mutated_name)
{
  Element *element;
  char * name;

  name = strdup("hello");
  element = doc.new_element(name);

  name[0] = 'y';
  ASSERT_EQ(element->name(), "hello");

  free(name);
}

TEST_F(ElementTest, set_attribute)
{
  Element *element;
  const char * const element_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  element = doc.new_element(element_name);

  element->set_attribute(attr_name, attr_value);
  ASSERT_EQ(element->get_attribute(attr_name), attr_value);
}

TEST_F(ElementTest, mutated_attribute)
{
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
  ASSERT_EQ(element->get_attribute("type"), "world");

  free(attr_name);
  free(attr_value);
}

TEST_F(ElementTest, foreach_attribute_enumerates_the_attributes)
{
  auto element = doc.new_element("hello");
  element->set_attribute("hello", "world");

  int counter = 0;
  auto counter_fn = [&](Node *attr) {
    counter++;
  };
  element->foreach_attribute(counter_fn);
  ASSERT_EQ(1, counter);
}

TEST_F(ElementTest, output_basic)
{
  Element *element;
  StringOutput out;

  element = doc.new_element("one");

  element->output(out);
  ASSERT_STREQ("<one />", out.string());
}

TEST_F(ElementTest, output_attribute)
{
  Element *element;
  StringOutput out;

  element = doc.new_element("one");
  element->set_attribute("hi", "there");

  element->output(out);
  ASSERT_STREQ("<one hi=\"there\" />", out.string());
}

TEST_F(ElementTest, output_child)
{
  Element *element;
  Element *child;
  StringOutput out;

  element = doc.new_element("one");
  child = doc.new_element("two");
  element->append_child(child);

  element->output(out);
  ASSERT_STREQ("<one><two /></one>", out.string());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
