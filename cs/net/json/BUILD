package(default_visibility = ["//visibility:public"])

cc_library(
    name = "object",
    hdrs = ["object.hh"],
    deps = [
        "//cs/result",
        "//cs/sanity:ensure",
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
