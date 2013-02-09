#include "intern.h"

#include "gmock/gmock.h"
#include <iostream>

class InternTest : public ::testing::Test {
protected:
  intern_t *i;

  void SetUp() {
    i = intern_new();
  }

  void TearDown() {
    intern_free(i);
  }
};

TEST_F(InternTest, create)
{
  ASSERT_NE(nullptr, i);
}

TEST_F(InternTest, const_string)
{
  const char * const input = "hello";
  const char *result;

  result = intern_intern(i, input);

  ASSERT_STREQ(input, result);
  ASSERT_NE(result, input);
}

TEST_F(InternTest, allocated_string)
{
  char *input;
  const char *result;

  input = strdup("hello");
  result = intern_intern(i, input);

  ASSERT_STREQ(input, result);
  ASSERT_NE(result, input);

  free(input);
}

TEST_F(InternTest, two_identical_strings)
{
  char *input1;
  char *input2;
  const char *result1;
  const char *result2;

  input1 = strdup("hello");
  input2 = strdup("hello");
  result1 = intern_intern(i, input1);
  result2 = intern_intern(i, input2);

  ASSERT_STREQ(input1, result1);
  ASSERT_STREQ(input2, result2);
  ASSERT_STREQ(result2, result1);
  ASSERT_EQ(result1, result2);

  free(input1);
  free(input2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
