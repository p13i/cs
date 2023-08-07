#include "cs/string/format.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::str::format;

TEST(Format, Empty) { EXPECT_EQ(format(""), ""); }

TEST(Format, Number) { EXPECT_EQ(format("%d", 1), "1"); }
