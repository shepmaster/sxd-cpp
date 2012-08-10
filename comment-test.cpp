#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "comment.h"

TEST_GROUP(comment)
{
};

TEST(comment, new_comment)
{
  Document doc;
  Comment *c;

  c = doc.new_comment("hello");

  CHECK(c != NULL);
  STRCMP_EQUAL("hello", c->text());
  CHECK_EQUAL(NODE_TYPE_COMMENT, c->type());

  delete c;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
