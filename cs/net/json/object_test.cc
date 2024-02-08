#include "cs/net/json/object.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::net::json::Object;
using ::cs::net::json::Type;
using ::cs::result::Result;
using ::testing::Eq;
using ::testing::IsTrue;
using ::testing::IsFalse;

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
