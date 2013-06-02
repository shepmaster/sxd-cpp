#include "attribute.h"

#include "document.h"
#include "test-utilities.h"

#include "gmock/gmock.h"
#include <iostream>

class AttributeTest : public ::testing::Test {
protected:
  Document doc;
  Element *element = doc.new_element("element");
};

TEST_F(AttributeTest, new_attribute)
{
  Attribute *attribute = doc.new_attribute(*element, "name", "value");

  ASSERT_NE(nullptr, attribute);
}

TEST_F(AttributeTest, attribute_delegates_namespaces_to_element)
{
  element->set_namespace_prefix("ns", "namespace-uri");
  auto *attribute = doc.new_attribute(*element, "name", "value");

  ASSERT_EQ("namespace-uri", *attribute->find_namespace_for_prefix("ns"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
