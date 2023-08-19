package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sinusodal",
    srcs = ["sinusodal.cc"],
    hdrs = ["sinusodal.h"],
    deps = [
        "//cs/math:constants",
    ],
)

cc_test(
    name = "sinusodal_test",
    srcs = ["sinusodal_test.cc"],
    deps = [
        ":sinusodal",
        "//cs/precision:floats",
        "@googletest//:gtest_main",
    ],
)
