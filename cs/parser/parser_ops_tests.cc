#include "gtest.h"
#include "parser_tests.h"
#include "runtime.h"

TEST(Get, OneArg_ShouldPass) {
  EXPECT_EVALUATION("exe(var(A),get(A))", true, 0);
}

TEST(Get, NoArgs_ShouldFail) {
  EXPECT_EVALUATION("exe(var())", false, 0);
}

TEST(Get, WithUnderscore) {
  EXPECT_EVALUATION("var(A_A)", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A_A"));
}

TEST(Get, ComplexName) {
  EXPECT_EVALUATION("var(A_BEFORE__)", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A_BEFORE__"));
}

TEST(Get, WithSpacesEverywhere_ShouldPass) {
  EXPECT_EVALUATION("exe(var(A),get( A ) )", true, 0);
}

TEST(Get, NonAllocVar_ShouldFail) {
  EXPECT_EVALUATION("exe(get(A))", false, 0);
}

TEST(Set, NoArgs_ShouldFail) {
  EXPECT_EVALUATION("exe(set())", false, 0);
}

TEST(Set, OneArg_ShouldFail) {
  EXPECT_EVALUATION("exe(set(A))", false, 0);
}

TEST(Set, HappyPath_ShouldPass) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A,2)
        )
    )",
                    true, 0);
}

TEST(Set, NonAllocVar_ShouldFail) {
  EXPECT_EVALUATION("exe(set(A,2))", false, 0);
}

TEST(Set, WithSpacesEverywhere_ShouldPass) {
  EXPECT_EVALUATION("exe(var(A),get( A ) )", true, 0);
}

TEST(SetThenGet, HappyPath_ShouldPass) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A,3),
            get(A)
        )
    )",
                    true, 3);
}

TEST(SetThenGet, TwoVars_ShouldPass) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            var(B),
            var(Z),

            set(A,3),
            set(B,6),

            set(Z,add(get(A),get(B))),
            get(Z)
        )
    )",
                    true, 9);
}

TEST(Var, BaseCases) {
  EXPECT_EVALUATION("var(A)", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_EVALUATION(" var( A ) ", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
}

TEST(Var, MultipleVars) {
  EXPECT_EVALUATION("exe(var(A),var(B),var(C))", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_TRUE(Runtime_IsVarAlloc("B"));
  EXPECT_TRUE(Runtime_IsVarAlloc("C"));
}

TEST(Set, BaseCases) {
  int result;

  EXPECT_EVALUATION("exe(var(A),set(A,1))", true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_TRUE(Runtime_TryGetVarValue("A", &result));
  EXPECT_EQ(1, result);

  EXPECT_EVALUATION("exe(var(A) , \nset( A \n,\n 1 ))",
                    true, 0);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_TRUE(Runtime_TryGetVarValue("A", &result));
  EXPECT_EQ(1, result);
}

TEST(Get, BaseCase) {
  int result;
  EXPECT_EVALUATION("exe(var(A),set(A,1),get(A))", true, 1);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_TRUE(Runtime_TryGetVarValue("A", &result));
  EXPECT_EQ(1, result);
}

TEST(Get, BaseCaseWithWhitespace) {
  int result;
  EXPECT_EVALUATION(
      "\n  exe(\n  var(A) , \n set( A , 1 ) \n,\n get( A "
      "))",
      true, 1);
  EXPECT_TRUE(Runtime_IsVarAlloc("A"));
  EXPECT_TRUE(Runtime_TryGetVarValue("A", &result));
  EXPECT_EQ(1, result);
}
