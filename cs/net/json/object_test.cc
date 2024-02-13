#include "cs/net/json/object.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::net::json::Object;
using ::cs::net::json::ParseBoolean;
using ::cs::net::json::ParseFloat;
using ::cs::net::json::ParseObject;
using ::cs::net::json::ParseString;
using ::cs::net::json::Type;
using ::cs::result::Result;
using ::testing::Eq;
using ::testing::FloatEq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::Matcher;

// TEST(ParseBoolean, EmptyString) {
//   uint cursor = 0;
//   auto result = ParseBoolean("", &cursor);
//   ASSERT_THAT(result.result().ok(), IsFalse());
// }

// TEST(ParseBoolean, True) {
//   uint cursor = 0;
//   auto result = ParseBoolean("true", &cursor);
//   ASSERT_THAT(result.result().ok(), IsTrue());
//   ASSERT_THAT(result.data(), IsTrue());
// }

TEST(ParseBoolean, True) {
  uint cursor = 0;
  auto result = ParseBoolean("true", &cursor);
  EXPECT_THAT(result.result().ok(), IsTrue());
  EXPECT_THAT(result.value(), IsTrue());
  EXPECT_THAT(cursor, Eq(4));
}

TEST(ParseBoolean, TrueWithStuffAfter) {
  uint cursor = 0;
  auto result = ParseBoolean("trueabc", &cursor);
  EXPECT_THAT(result.result().ok(), IsTrue());
  EXPECT_THAT(cursor, Eq(4));
  EXPECT_THAT(result.data(), IsTrue());
}

TEST(ParseBoolean, FalseWithStuffAfter) {
  uint cursor = 0;
  auto result = ParseBoolean("falseabc", &cursor);
  EXPECT_THAT(result.result().ok(), IsTrue());
  EXPECT_THAT(cursor, Eq(5));
  EXPECT_THAT(result.data(), IsFalse());
}

TEST(ParseBoolean, TrueWithWhitespaceBeforeShouldError) {
  uint cursor = 0;
  auto result = ParseBoolean(" false", &cursor);
  EXPECT_THAT(result.result().ok(), IsFalse());
  EXPECT_THAT(cursor, Eq(0));
}

TEST(ParseFloat, Float0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("0", &cursor).value(), FloatEq(0));
}

TEST(ParseFloat, Float0_1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("0.1", &cursor).value(), FloatEq(0.1));
}

TEST(ParseFloat, Float1_1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("1.1", &cursor).value(), FloatEq(1.1));
}

TEST(ParseFloat, FloatNeg0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("-0", &cursor).value(), FloatEq(0));
}

TEST(ParseFloat, FloatPos0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("+0", &cursor).value(), FloatEq(0));
}

TEST(ParseFloat, Float100) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("100", &cursor).value(), FloatEq(100));
}

TEST(ParseFloat, Float100_001) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("100.001", &cursor).value(),
              FloatEq(100.001));
}

TEST(ParseFloat, Float1e1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("1e0", &cursor).value(), FloatEq(1));
}

TEST(ParseFloat, Float12e2) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("12e2", &cursor).value(), FloatEq(1200));
}
