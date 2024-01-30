#include "gtest.h"
#include "parser_tests.h"
#include "runtime.h"

TEST(equ, base_case) {
  EXPECT_EVALUATION("equ(0,0)", true, 1);
  EXPECT_EVALUATION("equ(0,1)", true, 0);
  EXPECT_EVALUATION("equ(1,1)", true, 1);
}

TEST(equ, with_spaces) {
  EXPECT_EVALUATION(" equ( 0 , 0 ) ", true, 1);
}

TEST(equ, inside_iff) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A, 0),

            iff( equ( get( A ) , 0 ) , exe(
                set(A, add(get(A), 1)),
                set(A, add(get(A), 1))
            )),

            get(A)
        )
    )",
                    true, 2);
}

TEST(equ, inside_ife) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A, 0),

            ife( equ( get( A ) , 1 ) , exe(
                set(A, add(get(A), 1)),
                set(A, add(get(A), 1))
            ) , exe(
                set(A, add(get(A), 1))
            )),

            get(A)
        )
    )",
                    true, 1);
}
