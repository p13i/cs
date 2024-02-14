#include "cs/net/json/parsers.hh"

#include "cs/net/json/object.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::net::json::Object;
using ::cs::net::json::Type;
using ::cs::net::json::parsers::ParseArray;
using ::cs::net::json::parsers::ParseBoolean;
using ::cs::net::json::parsers::ParseFloat;
using ::cs::net::json::parsers::ParseObject;
using ::cs::net::json::parsers::ParseString;
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
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(result.value(), IsTrue());
  EXPECT_THAT(_cursor, Eq(4));
}

TEST_F(ParseBooleanTest, False) {
  auto result = ParseBoolean("false", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(result.value(), IsFalse());
  EXPECT_THAT(_cursor, Eq(5));
}

TEST_F(ParseBooleanTest, TrueWithStuffAfter) {
  auto result = ParseBoolean("trueabc", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(_cursor, Eq(4));
  EXPECT_THAT(result.data(), IsTrue());
}

TEST_F(ParseBooleanTest, FalseWithStuffAfter) {
  auto result = ParseBoolean("falseabc", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(_cursor, Eq(5));
  EXPECT_THAT(result.data(), IsFalse());
}

TEST_F(ParseBooleanTest,
       TrueWithWhitespaceBeforeShouldError) {
  auto result = ParseBoolean(" false", &_cursor);
  ASSERT_THAT(result.ok(), IsFalse());
  EXPECT_THAT(_cursor, Eq(0));
}

class ParseFloatTest : public ParseTest {};

TEST_F(ParseFloatTest, Float0) {
  EXPECT_THAT(ParseFloat("0", &_cursor).value(),
              FloatEq(0));
  EXPECT_THAT(_cursor, Eq(1));
}

TEST_F(ParseFloatTest, Float1) {
  EXPECT_THAT(ParseFloat("1", &_cursor).value(),
              FloatEq(1));
  EXPECT_THAT(_cursor, Eq(1));
}

TEST_F(ParseFloatTest, Float0_1) {
  EXPECT_THAT(ParseFloat("0.1", &_cursor).value(),
              FloatEq(0.1));
  EXPECT_THAT(_cursor, Eq(3));
}

TEST_F(ParseFloatTest, Float1_1) {
  EXPECT_THAT(ParseFloat("1.1", &_cursor).value(),
              FloatEq(1.1));
  EXPECT_THAT(_cursor, Eq(3));
}

TEST_F(ParseFloatTest, FloatNeg0) {
  EXPECT_THAT(ParseFloat("-0", &_cursor).value(),
              FloatEq(0));
  EXPECT_THAT(_cursor, Eq(2));
}

TEST_F(ParseFloatTest, FloatPos0) {
  EXPECT_THAT(ParseFloat("+0", &_cursor).value(),
              FloatEq(0));
  EXPECT_THAT(_cursor, Eq(2));
}

TEST_F(ParseFloatTest, Float100) {
  EXPECT_THAT(ParseFloat("100", &_cursor).value(),
              FloatEq(100));
  EXPECT_THAT(_cursor, Eq(3));
}

TEST_F(ParseFloatTest, Float100_001) {
  EXPECT_THAT(ParseFloat("100.001", &_cursor).value(),
              FloatEq(100.001));
  EXPECT_THAT(_cursor, Eq(7));
}

TEST_F(ParseFloatTest, Float1e1) {
  EXPECT_THAT(ParseFloat("1e0", &_cursor).value(),
              FloatEq(1));
  EXPECT_THAT(_cursor, Eq(3));
}

TEST_F(ParseFloatTest, Float12e2) {
  EXPECT_THAT(ParseFloat("12e2", &_cursor).value(),
              FloatEq(1200));
  EXPECT_THAT(_cursor, Eq(4));
}

class ParseStringTest : public ParseTest {};

TEST_F(ParseStringTest, EmptyString) {
  EXPECT_THAT(ParseString("\"\"", &_cursor).value(),
              StrEq(""));
  EXPECT_THAT(_cursor, Eq(2));
}

TEST_F(ParseStringTest, abc) {
  EXPECT_THAT(ParseString("\"abc\"", &_cursor).value(),
              StrEq("abc"));
  EXPECT_THAT(_cursor, Eq(5));
}

TEST_F(ParseStringTest, abcWithSpaces) {
  EXPECT_THAT(ParseString("\" a b c \"", &_cursor).value(),
              StrEq(" a b c "));
  EXPECT_THAT(_cursor, Eq(9));
}

TEST_F(ParseStringTest, Newline) {
  EXPECT_THAT(ParseString("\"\n\"", &_cursor).value(),
              StrEq("\n"));
  EXPECT_THAT(_cursor, Eq(3));
}

class ParseArrayTest : public ParseTest {};

TEST_F(ParseArrayTest, EmptyArray) {
  auto result = ParseObject("[]", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue()) << result;
  EXPECT_THAT(result.value()->type(), Eq(Type::ARRAY));
  EXPECT_THAT(result.value()->as_array().size(), Eq(0));
  EXPECT_THAT(_cursor, Eq(2));
}

TEST_F(ParseArrayTest, ArrayWithOneElement) {
  auto result = ParseArray("[1]", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue()) << result;
  auto array = result.value();
  EXPECT_THAT(array.size(), Eq(1));
  EXPECT_THAT(array.at(0)->type(), Eq(Type::NUMBER));
  EXPECT_THAT(array.at(0)->as_number(), FloatEq(1));
  EXPECT_THAT(_cursor, Eq(3));
}

TEST_F(ParseArrayTest, ArrayWithTwoElements) {
  auto result = ParseObject("[1,2]", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue()) << result;
  EXPECT_THAT(result.value()->type(), Eq(Type::ARRAY));
  auto array = result.value()->as_array();
  EXPECT_THAT(array.size(), Eq(2));
  EXPECT_THAT(array.at(0)->type(), Eq(Type::NUMBER));
  EXPECT_THAT(array.at(0)->as_number(), FloatEq(1));
  EXPECT_THAT(array.at(1)->type(), Eq(Type::NUMBER));
  EXPECT_THAT(array.at(1)->as_number(), FloatEq(2));
  EXPECT_THAT(_cursor, Eq(5));
}

TEST_F(ParseArrayTest, ArrayWithOneStringAndOneFloatAndOneBool) {
  auto result = ParseObject("[\"abc\",1.1,true]", &_cursor);
  ASSERT_THAT(result.ok(), IsTrue()) << result;
  auto array = result.value();
  EXPECT_THAT(array->type(), Eq(Type::ARRAY));
  EXPECT_THAT(array->as_array().size(), Eq(3));
  EXPECT_THAT(array->as_array().at(0)->type(), Eq(Type::STRING));
  EXPECT_THAT(array->as_array().at(0)->as_string(), StrEq("abc"));
  EXPECT_THAT(array->as_array().at(1)->type(), Eq(Type::NUMBER));
  EXPECT_THAT(array->as_array().at(1)->as_number(), FloatEq(1.1));
  EXPECT_THAT(array->as_array().at(2)->type(), Eq(Type::BOOLEAN));
  EXPECT_THAT(array->as_array().at(2)->as_bool(), IsTrue());
  EXPECT_THAT(_cursor, Eq(16));
}

TEST_F(ParseArrayTest, NestedArray) {
  auto result = ParseObject("[[1,2],[3,4]]", &_cursor);
  ASSERT_TRUE(result.ok()) << result;

  auto array = result.value();
  EXPECT_EQ(array->type(), Type::ARRAY);
  EXPECT_EQ(array->as_array().size(), 2);

  auto subArray1 = array->as_array().at(0)->as_array();
  EXPECT_EQ(subArray1.size(), 2);
  EXPECT_EQ(subArray1.at(0)->type(), Type::NUMBER);
  EXPECT_FLOAT_EQ(subArray1.at(0)->as_number(), 1);
  EXPECT_EQ(subArray1.at(1)->type(), Type::NUMBER);
  EXPECT_FLOAT_EQ(subArray1.at(1)->as_number(), 2);

  auto subArray2 = array->as_array().at(1)->as_array();
  EXPECT_EQ(subArray2.size(), 2);
  EXPECT_EQ(subArray2.at(0)->type(), Type::NUMBER);
  EXPECT_FLOAT_EQ(subArray2.at(0)->as_number(), 3);
  EXPECT_EQ(subArray2.at(1)->type(), Type::NUMBER);
  EXPECT_FLOAT_EQ(subArray2.at(1)->as_number(), 4);

  EXPECT_EQ(_cursor, 13);
}
