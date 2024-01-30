#include "gtest.h"
#include "parser_tests.h"
#include "runtime.h"
#include "utils_strings.h"

TEST(HasPrefix, BaseCase)
{
    EXPECT_TRUE(Utils_Strings_HasPrefix("a", "a"));
    EXPECT_TRUE(Utils_Strings_HasPrefix("add(1,1)", "add("));
    EXPECT_TRUE(Utils_Strings_HasPrefix("abc", "ab"));
    EXPECT_FALSE(Utils_Strings_HasPrefix("abc", "c"));
    EXPECT_FALSE(Utils_Strings_HasPrefix("abc", "ccc"));
}
