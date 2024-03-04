package(default_visibility = ["//visibility:public"])

cc_library(
    name = "object",
    hdrs = ["object.hh"],
    deps = [
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_test(
    name = "object_test",
    srcs = ["object_test.cc"],
    deps = [
        "//cs/net/json:object",
        "//cs/result",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "parsers",
    srcs = ["parsers.cc"],
    hdrs = ["parsers.hh"],
    deps = [
        "//cs/net/json:object",
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_test(
    name = "parsers_test",
    srcs = ["parsers_test.cc"],
    deps = [
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/result",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "serialize",
    srcs = ["serialize.cc"],
    hdrs = ["serialize.hh"],
    deps = [
        "//cs/net/json:object",
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_test(
    name = "serialize_test",
    srcs = ["serialize_test.cc"],
    deps = [
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/net/json:serialize",
        "//cs/result",
        "@googletest//:gtest_main",
    ],
)
