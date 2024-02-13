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
using ::testing::StrEq;

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

TEST_F(ParseBooleanTest,
       TrueWithWhitespaceBeforeShouldError) {
  auto result = ParseBoolean(" false", &_cursor);
  EXPECT_THAT(result.ok(), IsFalse());
  EXPECT_THAT(_cursor, Eq(0));
}

class ParseFloatTest : public ParseTest {};

TEST_F(ParseFloatTest, Float0) {
  EXPECT_THAT(ParseFloat("0", &_cursor).value(), FloatEq(0));
}

TEST_F(ParseFloatTest, Float0_1) {
  EXPECT_THAT(ParseFloat("0.1", &_cursor).value(),
              FloatEq(0.1));
}

TEST_F(ParseFloatTest, Float1_1) {
  EXPECT_THAT(ParseFloat("1.1", &_cursor).value(),
              FloatEq(1.1));
}

TEST_F(ParseFloatTest, FloatNeg0) {
  EXPECT_THAT(ParseFloat("-0", &_cursor).value(),
              FloatEq(0));
}

TEST_F(ParseFloatTest, FloatPos0) {
  EXPECT_THAT(ParseFloat("+0", &_cursor).value(),
              FloatEq(0));
}

TEST_F(ParseFloatTest, Float100) {
  EXPECT_THAT(ParseFloat("100", &_cursor).value(),
              FloatEq(100));
}

TEST_F(ParseFloatTest, Float100_001) {
  EXPECT_THAT(ParseFloat("100.001", &_cursor).value(),
              FloatEq(100.001));
}

TEST_F(ParseFloatTest, Float1e1) {
  EXPECT_THAT(ParseFloat("1e0", &_cursor).value(),
              FloatEq(1));
}

TEST_F(ParseFloatTest, Float12e2) {
  EXPECT_THAT(ParseFloat("12e2", &_cursor).value(),
              FloatEq(1200));
}

class ParseStringTest : public ParseTest {};

TEST_F(ParseStringTest, EmptyString) {
  EXPECT_THAT(*ParseString("\"\"", &_cursor).value(),
              StrEq(""));
}

TEST_F(ParseStringTest, abc) {
  EXPECT_THAT(*ParseString("\"abc\"", &_cursor).value(),
              StrEq("abc"));
}

TEST_F(ParseStringTest, abcWithSpaces) {
  EXPECT_THAT(*ParseString("\" a b c \"", &_cursor).value(),
              StrEq(" a b c "));
}

TEST_F(ParseStringTest, Newline) {
  EXPECT_THAT(*ParseString("\"\n\"", &_cursor).value(),
              StrEq("\n"));
}
