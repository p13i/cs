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
    ],
    hdrs = [
        "debug.h",
        "parser.h",
        "parser_expectations.h",
        "runtime.h",
    ],
    deps = [":utils_strings"],
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
    hdrs = ["parser_tests.h"],
)

cc_library(
    name = "parser_test",
    srcs = [
        "parser_conditionals_tests.cc",
        "parser_ops_equ_tests.cc",
        "parser_ops_for_tests.cc",
        "parser_ops_tests.cc",
        "parser_tests.cc",
    ],
    deps = [
        ":gtest",
        ":parser",
        ":parser_test_library",
    ],
)

cc_library(
    name = "gtest",
    srcs = ["gtest-all.cc"],
    hdrs = ["gtest.h"],
)
