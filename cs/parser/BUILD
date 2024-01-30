cc_binary(
    name="main",
    deps=[":parser"]
)

cc_library(
   name= "parser",
    hdrs=[
        "debug.h",
"parser_expectations.h",
"parser.h",
"runtime.h",
    ],
    srcs=[
"parser.c",
"parser_consumers.c",
"runtime.c",
"utils_strings.c",
"utils_strings.h",
    ]
)

cc_library(
   name= "parser_test_library",
   hdrs=[
"parser_tests.h",
   ],
   srcs=[
"parser_ops_equ_tests.cc",
"parser_ops_for_tests.cc",
"parser_conditionals_tests.cc",
"parser_ops_tests.cc",
"parser_tests.cc",
"utils_strings_test.cc",],
    deps=[
":parser",
":gtest"
    ]
)

cc_library(
    name="gtest",
    hdrs=["gtest.h"],
    srcs=["gtest-all.cc"],
)
