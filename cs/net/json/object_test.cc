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

TEST(Parse, BaseCase) {
  Object* object = new Object();
  std::string input = "";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(*object, Eq(Object()));
}

TEST(Parse, BooleanTrue) {
  Object* object = new Object();
  std::string input = "true";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::BOOLEAN));
  EXPECT_THAT(object->as_bool(), IsTrue());
}

TEST(Parse, BooleanFalse) {
  Object* object = new Object();
  std::string input = "false";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::BOOLEAN));
  EXPECT_THAT(object->as_bool(), IsFalse());
}

TEST(Parse, Float0) {
  Object* object = new Object();
  std::string input = "0";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), Eq(0));
}

TEST(Parse, Float0_1) {
  Object* object = new Object();
  std::string input = "0.1";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(0.1));
}

TEST(Parse, Float1_1) {
  Object* object = new Object();
  std::string input = "1.1";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(1.1));
}

TEST(Parse, FloatNeg0) {
  Object* object = new Object();
  std::string input = "-0";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(0));
}

TEST(Parse, FloatPos0) {
  Object* object = new Object();
  std::string input = "+0";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(0));
}

TEST(Parse, Float100) {
  Object* object = new Object();
  std::string input = "100";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(100));
}

TEST(Parse, Float100_001) {
  Object* object = new Object();
  std::string input = "100.001";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(100.001));
}

TEST(Parse, Float1e1) {
  Object* object = new Object();
  std::string input = "1e0";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(1));
}

TEST(Parse, Float12e2) {
  Object* object = new Object();
  std::string input = "12e2";
  Result result = input >> object;
  ASSERT_THAT(result.ok(), IsTrue());
  EXPECT_THAT(object->type(), Eq(Type::NUMBER));
  EXPECT_THAT(object->as_number(), FloatEq(1200));
}
