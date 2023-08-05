#include "cs/MyLib/message.hpp"

#include "gtest/gtest.h"

TEST(message_test, content) {
  EXPECT_EQ(get_message(), "Hello World!");
}
