#include "text-node.h"

#include "document.h"
#include "test-utilities.h"

#include "gmock/gmock.h"
#include <iostream>

class TextNodeTest : public ::testing::Test {
protected:
  Document doc;
};

TEST_F(TextNodeTest, new_text_node)
{
  TextNode *tn;

  tn = doc.new_text_node("I am text");

  ASSERT_EQ("I am text", tn->text());

  delete tn;
}

TEST_F(TextNodeTest, output)
{
  TextNode *tn;
  StringOutput out;

  tn = doc.new_text_node("I am text");

  tn->output(out);

  ASSERT_STREQ("I am text", out.string());

  delete tn;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
