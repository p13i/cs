#include "parser_tests.h"

#include "gtest.h"
#include "parser.h"

int Parser_Tests_main(int argc, char **argv) {
  printf(
      "Running Parser_Tests_Run() from parser_tests.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// write a basic gtest
TEST(BaseCase, NullInputString_ShouldFail) {
  EXPECT_EVALUATION((char *)NULL, false, 0);
}

TEST(BaseCase, EmptyString_ShouldFail) {
  EXPECT_EVALUATION("", false, 0);
}

TEST(BaseCase, OneSpace_ShouldFail) {
  EXPECT_EVALUATION(" ", false, 0);
}

TEST(BaseCase, TwoSpaces_ShouldFail) {
  EXPECT_EVALUATION("  ", false, 0);
}

TEST(BaseCase, VarName_ShouldFail) {
  EXPECT_EVALUATION("A", false, 0);
}

TEST(BaseCase, Underscore_ShouldFail) {
  EXPECT_EVALUATION("_", false, 0);
}

TEST(Operator, SingleMinus_ShouldFail) {
  EXPECT_EVALUATION("-", false, 0);
}

TEST(Operator, SinglePlus_ShouldFail) {
  EXPECT_EVALUATION("+", false, 0);
}

TEST(Number, One) { EXPECT_EVALUATION("1", true, 1); }

TEST(Number, OneWithSpaceInFront) {
  EXPECT_EVALUATION(" 1", true, 1);
}

TEST(Number, OneWithSpaceInBack) {
  EXPECT_EVALUATION("1 ", true, 1);
}

TEST(Number, OneWithSpacesAround) {
  EXPECT_EVALUATION(" 1 ", true, 1);
}

TEST(Number, NegativeOne) {
  EXPECT_EVALUATION("-1", true, -1);
}

TEST(Number, TwoDigits) {
  EXPECT_EVALUATION("10", true, 10);
}

TEST(Number, TwoDigitsNegative) {
  EXPECT_EVALUATION("-10", true, -10);
}

TEST(Number, FiveDigits) {
  EXPECT_EVALUATION("12345", true, 12345);
}

TEST(Number, FiveDigitsNegative) {
  EXPECT_EVALUATION("-12345", true, -12345);
}

TEST(Number, NegativeLargeNumber1) {
  EXPECT_EVALUATION("-1893943", true, -1893943);
}

TEST(Number, PositiveLargeNumber) {
  EXPECT_EVALUATION("+348234", true, 348234);
}

TEST(Operator, Negation) {
  EXPECT_EVALUATION("neg(1)", true, -1);
}

TEST(Operator, NegationWithSpace) {
  EXPECT_EVALUATION(" neg(1)", true, -1);
}

TEST(Operator, NegationWithSpaceInside1) {
  EXPECT_EVALUATION("neg( 1)", true, -1);
}

TEST(Operator, NegationWithSpaceInside2) {
  EXPECT_EVALUATION("neg(1 )", true, -1);
}

TEST(Operator, DoubleNegative) {
  EXPECT_EVALUATION("neg(-1)", true, 1);
}

TEST(Operator, Add) {
  EXPECT_EVALUATION("add(1,1)", true, 2);
}

TEST(Operator, AddWithSpaces) {
  EXPECT_EVALUATION("add( 1 , 1 )", true, 2);
}

TEST(Operator, Sub) {
  EXPECT_EVALUATION("sub(1,1)", true, 0);
}

TEST(Operator, Mul) {
  EXPECT_EVALUATION("mul(12183,54)", true, 657882);
}

TEST(Operator, Div) {
  EXPECT_EVALUATION("div(12183,54)", true, 225);
}

TEST(Operator, Mul2) {
  EXPECT_EVALUATION("mul(1,3)", true, 3);
}

TEST(Operator, Embedded1) {
  EXPECT_EVALUATION("mul(add(1,1),3)", true, 6);
}

TEST(Operator, Embedded2) {
  EXPECT_EVALUATION("mul(add(1,2),add(3,4))", true, 21);
}

TEST(Operator, Embedded3) {
  EXPECT_EVALUATION("mul(add(1,2),mul(3,4))", true, 36);
}

TEST(Operator, Embedded4) {
  EXPECT_EVALUATION("add(add(1,1),add(1,add(1,1)))", true,
                    5);
}

TEST(Operator, Embedded5) {
  EXPECT_EVALUATION("add( add(1 ,1),add(1, add(1, 1) ) ) ",
                    true, 5);
}

TEST(Operator, Embedded6) {
  EXPECT_EVALUATION("sub(add(1,1),div(10,mul(1,2)))", true,
                    -3);
}

TEST(Operator, Embedded7) {
  EXPECT_EVALUATION("sub(add(1,1),div(9,mul(1,div(3,1))))",
                    true, -1);
}

TEST(Operator, Embedded8) {
  EXPECT_EVALUATION(
      "\n\tsub(add(1,1),div(9,mul(1,div(3,1))))", true, -1);
}

TEST(Execute, OneArgument) {
  EXPECT_EVALUATION("exe(1)", true, 1);
}

TEST(Execute, ShouldReturnLastValue) {
  EXPECT_EVALUATION("exe(1,2)", true, 2);
}

TEST(Execute, Return1Plus1) {
  EXPECT_EVALUATION("exe(add(1,1))", true, 2);
}

TEST(Execute, ExecuteWithSubExpressionAsArgument) {
  EXPECT_EVALUATION("exe(add(1,1),0)", true, 0);
}

TEST(Execute, ExecuteWithThreeSteps) {
  EXPECT_EVALUATION("exe(add(1,1),0,1)", true, 1);
}

TEST(Var, OneArgument) {
  EXPECT_EVALUATION("var(A)", true, 0);
}

TEST(Var, InsideExe) {
  EXPECT_EVALUATION("exe(var(A))", true, 0);
}

TEST(Var, TwoVars) {
  EXPECT_EVALUATION("exe(var(A),var(B))", true, 0);
}

TEST(Var, ThreeVarsWithSpaces) {
  EXPECT_EVALUATION("exe(var( A),var(B ) , var( C) ) ",
                    true, 0);
}

TEST(Var, ThreeVarsWithOpsAsArgs_ShouldFail) {
  EXPECT_EVALUATION(
      "exe(var( add(1,1)),var(B ) , var( C) ) ", false, 0);
}

TEST(Var, TwoLetterVarName_ShouldPass) {
  EXPECT_EVALUATION("exe(var(AA))", true, 0);
}

TEST(Var, TwoLetterVarNameWithUnderscore_ShouldPass) {
  EXPECT_EVALUATION("exe(var(A_A))", true, 0);
}

TEST(Var, UnderscoreName_ShouldPass) {
  EXPECT_EVALUATION("exe(var(_))", true, 0);
}

TEST(Var, EmptySpace_ShouldFail) {
  EXPECT_EVALUATION("exe(var( ))", false, 0);
}

TEST(Complex1, ShouldPass) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            var(B),
            var(C),
            var(D),
            var(Z),

            set(A,3),
            set(B,6),
            set(C,mul(get(A),get(B))),
            set(D,2),

            set(Z,div(get(C),9)),
            get(Z)
        )
    )",
                    true, 2);
}

TEST(Complex2, ShouldPass) {
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A,3),
            set(A,add(get(A),1)),
            get(A)
        )
    )",
                    true, 4);
}

TEST(Complex3, ShouldPass) {
  //
  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            var(B),

            set(A,3),
            set(B,4),

            set( B , add( get( B ) , 1 ) ),
            
            set(Z, exe(
                var(Z),
                set(Z, add(get(B),3)),
                get(Z)
            )),

            set(Z, mul(add(get(A),get(B)),get(Z))),

            get(Z)
        )
    )",
                    true, 64);
}

TEST(Complex4, ShouldPass) {
  /*

  fxn(ADD_THREE, [S], exe(
      set(S, ADDTHREE(S)),
      get(S)
  )),

  fxn(CALLS_ADD_THREE, [A_BEFORE], exe(
      var(A_AFTER),
      set(A_AFTER, ADDTHREE[A_BEFORE]),
      ret(A_AFTER)    // can be omitted
  )),

  fxn(TAKES_TWO_ARGS, [A1, A2], exe(
      var(RESULT),
      set(RESULT, mul(ADDTHREE[A1], CALLS_ADD_THREE[A2])),
      ret(A_AFTER)    // can be omitted
  )),

  */
  // set(A, ADD_THREE[A])

  EXPECT_EVALUATION(R"(
        exe(
            var(A),
            set(A,0),
            set(A,add(get(A),1)),
            set(A,add(get(A),1)),
            set(A,add(get(A),1)),
            get(A)
        )
    )",
                    true, 3);
}
