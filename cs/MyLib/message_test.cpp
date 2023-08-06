#include "cs/MyLib/message.h"

#include "gtest/gtest.h"

TEST(message_test, content) {
  EXPECT_EQ(get_message(), "Hello World!");
}
