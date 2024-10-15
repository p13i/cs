package(default_visibility = ["//visibility:public"])

cc_library(
    name = "format",
    hdrs = [
        "format.h",
        "format.hh",
    ],
    copts = ["-Wno-format-security"],
)

cc_test(
    name = "format_test",
    srcs = ["format_test.cc"],
    copts = ["-Wno-format-security"],
    deps = [
        ":format",
        "@googletest//:gtest_main",
    ],
)
