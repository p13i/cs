#ifndef PARSER_TESTS
#define PARSER_TESTS

#include "gtest.h"
#include "parser.h"

int Parser_Tests_main(int argc, char **argv);

#define EXPECT_EVALUATION(string, expectedSuccess, expectedResult) \
    {   \
        int result = 0; \
        bool success = Parser_TryEvaluate(string, &result); \
        EXPECT_EQ(expectedSuccess, success);    \
        EXPECT_EQ(expectedResult, result);      \
    }

#endif // PARSER_TESTS
