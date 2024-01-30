#include "gtest/gtest.h"
extern "C" {
#include "parser_tests.h"
#include "runtime.h"
}
#if 0
// for loop syntax
// for( VAR_INIT, TERM_COND, NEXT_STEP, EXPR_BLOCK )
TEST(for_, base_case)
{
    EXPECT_EVALUATION(R"(
        for( var(IDX) , neg(equ(IDX,5)) , set(IDX,add(IDX,1)) , exe(
            add(1,1)
        ))
    )", true, 2);
}
#endif
