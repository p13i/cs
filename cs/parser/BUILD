cc_binary(
    name = "main",
    deps = [":parser"],
)

cc_library(
    name = "parser",
    srcs = [
        "parser.c",
        "parser_consumers.c",
        "runtime.c",
        "utils_strings.c",
        "utils_strings.h",
    ],
    hdrs = [
        "debug.h",
        "parser.h",
        "parser_expectations.h",
        "runtime.h",
    ],
)

cc_library(
    name = "utils_strings",
    srcs = ["utils_strings.c"],
    hdrs = ["utils_strings.h"],
)

cc_test(
    name = "utils_strings_test",
    srcs = ["utils_strings_test.cc"],
    deps = [
        ":utils_strings",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "parser_test_library",
    srcs = [
        "parser_conditionals_tests.cc",
        "parser_ops_equ_tests.cc",
        "parser_ops_for_tests.cc",
        "parser_ops_tests.cc",
        "parser_tests.cc",
    ],
    hdrs = [
        "parser_tests.h",
    ],
    deps = [
        ":gtest",
        ":parser",
    ],
)

cc_library(
    name = "gtest",
    srcs = ["gtest-all.cc"],
    hdrs = ["gtest.h"],
)
