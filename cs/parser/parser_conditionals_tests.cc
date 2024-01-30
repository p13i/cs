#include "gtest/gtest.h"
#include "parser_tests.h"
#include "runtime.h"

#if 0
TEST(equ, base_case)
{
    EXPECT_EVALUATION("equ(0,0)", true, 1);
    EXPECT_EVALUATION("equ(0,1)", false, 0);
    EXPECT_EVALUATION("equ(1,1)", true, 1);
}

TEST(neq, base_case)
{
    EXPECT_EVALUATION("neq(0,0)", false, 0);
    EXPECT_EVALUATION("neq(0,1)", true, 1);
    EXPECT_EVALUATION("neq(1,1)", false, 0);
}
#endif

TEST(iff, base_case) {
  // Third arg is optional (else)
  EXPECT_EVALUATION("iff(1,exe())", true, 0);
}

TEST(iff, should_skip_block) {
  EXPECT_EVALUATION("iff(0,exe(add(1,1)))", true, 0);
}

TEST(iff, with_spaces) {
  EXPECT_EVALUATION(" iff( 1 , exe( ) ) ", true, 0);
}

TEST(iff, complex_1)
// "Else if" is done by putting iff into third arg of first
// iff call
EXPECT_EVALUATION(R"(
        iff(1, 
            exe(
                var(A),
                set(A, 1),
                get(A)
            )
        )
    )",
                  true, 1);

TEST(iff, inside_exe_block) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A, 1),
            iff(1, exe(
                set(A, add(get(A), 2))
            )),
            get(A)
        )
    )",
                    true, 3);
}

TEST(ife, base_case) {
  EXPECT_EVALUATION("ife(0,exe(),exe())", true, 0);
}

TEST(ife, base_case_else) {
  EXPECT_EVALUATION("ife(0,exe(),exe())", true, 0);
}

TEST(ife, execute_true_block) {
  EXPECT_EVALUATION("ife(1,exe(mul(2,3)),exe())", true, 6);
}

TEST(ife, execute_false_block) {
  EXPECT_EVALUATION("ife(0,exe(),exe(add(1,1)))", true, 2);
}

TEST(ife, inside_exe_block) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A, 1),
            ife(0, exe(
                set(A, add(get(A), 2))
            ), add(get(A), 1))
        )
    )",
                    true, 2);
}

TEST(ife, inside_exe_block_with_new_var) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A, 1),
            ife(0, exe(
                set(A, add(get(A), 2)),
                set(A, add(get(A), 2))
            ), exe(
                var(B),
                set(B, 0),
                set(B, add(get(B), 1))
            )),
            get(B)
        )
    )",
                    true, 1);
}
