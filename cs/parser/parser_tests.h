#ifndef PARSER_TESTS
#define PARSER_TESTS

#include "gtest/gtest.h"

extern "C" {
#include "parser.h"
}

#define EXPECT_EVALUATION(string, expectedSuccess,      \
                          expectedResult)               \
  {                                                     \
    int result = 0;                                     \
    bool success = Parser_TryEvaluate(string, &result); \
    EXPECT_EQ(expectedSuccess, success);                \
    EXPECT_EQ(expectedResult, result);                  \
  }

#endif  // PARSER_TESTS
