#include "comment.h"

#include "document.h"
#include "make-unique.h"

#include "gmock/gmock.h"
#include <iostream>

class CommentTest : public ::testing::Test {
};

TEST_F(CommentTest, new_comment)
{
  Document doc;
  Comment *c;

  c = doc.new_comment("hello");

  ASSERT_TRUE(c != NULL);
  ASSERT_STREQ("hello", c->text());
  ASSERT_EQ(NODE_TYPE_COMMENT, c->type());

  delete c;
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
