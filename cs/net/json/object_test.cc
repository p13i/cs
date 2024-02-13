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
using ::testing::StrEq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::Matcher;

class ParseTest : public ::testing::Test {
 public:
  void SetUp() override { _cursor = 0; }

  uint _cursor;
};

class ParseBooleanTest : public ParseTest {};

TEST_F(ParseBooleanTest, EmptyString) {
  auto result = ParseBoolean("", &_cursor);
  ASSERT_THAT(result.ok(), IsFalse());
}

TEST_F(ParseBooleanTest, True) {
  auto result = ParseBoolean("true", &_cursor);
  EXPECT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(result.value(), IsTrue());
  EXPECT_THAT(_cursor, Eq(4));
}

TEST_F(ParseBooleanTest, False) {
  auto result = ParseBoolean("false", &_cursor);
  EXPECT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(result.value(), IsFalse());
  EXPECT_THAT(_cursor, Eq(5));
}

TEST_F(ParseBooleanTest, TrueWithStuffAfter) {
  auto result = ParseBoolean("trueabc", &_cursor);
  EXPECT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(_cursor, Eq(4));
  EXPECT_THAT(result.data(), IsTrue());
}

TEST_F(ParseBooleanTest, FalseWithStuffAfter) {
  auto result = ParseBoolean("falseabc", &_cursor);
  EXPECT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(_cursor, Eq(5));
  EXPECT_THAT(result.data(), IsFalse());
}

TEST_F(ParseBooleanTest, TrueWithWhitespaceBeforeShouldError) {
  auto result = ParseBoolean(" false", &_cursor);
  EXPECT_THAT(result.ok(), IsFalse());
  EXPECT_THAT(_cursor, Eq(0));
}

TEST(ParseFloat, Float0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("0", &cursor).value(), FloatEq(0));
}

TEST(ParseFloat, Float0_1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("0.1", &cursor).value(),
              FloatEq(0.1));
}

TEST(ParseFloat, Float1_1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("1.1", &cursor).value(),
              FloatEq(1.1));
}

TEST(ParseFloat, FloatNeg0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("-0", &cursor).value(),
              FloatEq(0));
}

TEST(ParseFloat, FloatPos0) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("+0", &cursor).value(),
              FloatEq(0));
}

TEST(ParseFloat, Float100) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("100", &cursor).value(),
              FloatEq(100));
}

TEST(ParseFloat, Float100_001) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("100.001", &cursor).value(),
              FloatEq(100.001));
}

TEST(ParseFloat, Float1e1) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("1e0", &cursor).value(),
              FloatEq(1));
}

TEST(ParseFloat, Float12e2) {
  uint cursor = 0;
  EXPECT_THAT(ParseFloat("12e2", &cursor).value(),
              FloatEq(1200));
}

class ParseStringTest : public ParseTest {};

TEST_F(ParseStringTest, EmptyString) {
  EXPECT_THAT(*ParseString("\"\"", &_cursor).value(), StrEq(""));
}

TEST_F(ParseStringTest, abc) {
  EXPECT_THAT(*ParseString("\"abc\"", &_cursor).value(), StrEq("abc"));
}

TEST_F(ParseStringTest, abcWithSpaces) {
  EXPECT_THAT(*ParseString("\" a b c \"", &_cursor).value(), StrEq(" a b c "));
}

TEST_F(ParseStringTest, Newline) {
  EXPECT_THAT(*ParseString("\"\n\"", &_cursor).value(), StrEq("\n"));
}
