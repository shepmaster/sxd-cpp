#include "xpath-value-nodeset.h"

#include "document.h"

#include "make-unique.h"
#include "gmock/gmock.h"
#include <iostream>

using testing::HasSubstr;

class XPathValueNodesetTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset nodes;
  std::string node_name = "a-node";
  Node *node = doc.new_element(node_name);

  XPathValueNodeset value() {
    return XPathValueNodeset(nodes);
  }
};

TEST_F(XPathValueNodesetTest, can_be_a_nodeset)
{
  ASSERT_EQ(nodes, value().nodeset());
}

TEST_F(XPathValueNodesetTest, knows_the_native_type_is_nodeset)
{
  ASSERT_TRUE(value().is(XPathValue::Type::Nodeset));
}

TEST_F(XPathValueNodesetTest, converts_empty_nodeset_to_false)
{
  ASSERT_EQ(false, value().boolean());
}

TEST_F(XPathValueNodesetTest, converts_non_empty_nodeset_to_true)
{
  nodes.add(node);
  ASSERT_EQ(true, value().boolean());
}

TEST_F(XPathValueNodesetTest, string_contains_useful_information)
{
  std::stringstream output;
  nodes.add(node);
  output << value();
  ASSERT_THAT(output.str(), HasSubstr("Nodeset"));
  ASSERT_THAT(output.str(), HasSubstr(node_name));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
