#include "cs/net/json/object.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::net::json::Object;
using ::cs::net::json::Type;
using ::cs::result::Result;
using ::testing::Eq;
using ::testing::FloatEq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::Matcher;

class ObjectParsingTest : public ::testing::Test {};

#define EXPECT_PARSE_OK(input, type, matcher)     \
  {                                               \
    Object* object = new Object();                \
    Result result = std::string(input) >> object; \
    ASSERT_THAT(result.ok(), IsTrue());           \
    EXPECT_THAT(object->as_##type(), matcher);    \
    delete object;                                \
  }

TEST_F(ObjectParsingTest, BooleanTrue) {
  EXPECT_PARSE_OK("true", bool, IsTrue());
}

TEST_F(ObjectParsingTest, BooleanFalse) {
  EXPECT_PARSE_OK("false", bool, IsFalse());
}

TEST_F(ObjectParsingTest, Float0) {
  EXPECT_PARSE_OK("0", number, Eq(0));
}

TEST_F(ObjectParsingTest, Float0_1) {
  EXPECT_PARSE_OK("0.1", number, FloatEq(0.1));
}

TEST_F(ObjectParsingTest, Float1_1) {
  EXPECT_PARSE_OK("1.1", number, FloatEq(1.1));
}

TEST_F(ObjectParsingTest, FloatNeg0) {
  EXPECT_PARSE_OK("-0", number, FloatEq(0));
}

TEST_F(ObjectParsingTest, FloatPos0) {
  EXPECT_PARSE_OK("+0", number, FloatEq(0));
}

TEST_F(ObjectParsingTest, Float100) {
  EXPECT_PARSE_OK("100", number, FloatEq(100));
}

TEST_F(ObjectParsingTest, Float100_001) {
  EXPECT_PARSE_OK("100.001", number, FloatEq(100.001));
}

TEST_F(ObjectParsingTest, Float1e1) {
  EXPECT_PARSE_OK("1e0", number, FloatEq(1));
}

TEST_F(ObjectParsingTest, Float12e2) {
  EXPECT_PARSE_OK("12e2", number, FloatEq(1200));
}

TEST_F(ObjectParsingTest, EmptyString) {
  EXPECT_PARSE_OK("\"\"", string, Eq("\"\""));
}

TEST_F(ObjectParsingTest, StringWithOneCharacter) {
  EXPECT_PARSE_OK("\"a\"", string, Eq("\"a\""));
}

TEST_F(ObjectParsingTest, StringAbc) {
  EXPECT_PARSE_OK("\"abc\"", string, Eq("\"abc\""));
}
