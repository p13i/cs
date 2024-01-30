cc_library(
    name = "parser",
    srcs = [
        "parser.c",
        "parser_consumers.c",
    ],
    hdrs = [
        "parser.h",
        "parser_expectations.h",
    ],
    deps = [
        ":debug_library",
        ":runtime",
        ":utils_strings",
    ],
)

cc_library(
    name = "debug_library",
    hdrs = ["debug.h"],
)

cc_library(
    name = "runtime",
    srcs = ["runtime.c"],
    hdrs = ["runtime.h"],
    deps = [
        ":debug_library",
        ":utils_strings",
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
    hdrs = ["parser_tests.h"],
)

cc_test(
    name = "parser_tests",
    srcs = [
        "parser_tests.cc",
    ],
    deps = [
        ":parser",
        ":parser_test_library",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_conditionals_test",
    srcs = [
        "parser_conditionals_tests.cc",
    ],
    deps = [
        ":parser",
        ":parser_test_library",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_ops_equ_test",
    srcs = [
        "parser_ops_equ_tests.cc",
    ],
    deps = [
        ":parser",
        ":parser_test_library",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_ops_for_tests",
    srcs = [
        "parser_ops_for_tests.cc",
    ],
    deps = [
        ":parser",
        ":parser_test_library",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_ops_tests",
    srcs = [
        "parser_ops_tests.cc",
    ],
    deps = [
        ":parser",
        ":parser_test_library",
        "@googletest//:gtest_main",
    ],
)
