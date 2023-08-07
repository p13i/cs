package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constants",
    hdrs = ["constants.h"],
)

cc_library(
    name = "polar2cart",
    hdrs = ["polar2cart.h"],
)

cc_library(
    name = "random",
    srcs = ["random.cc"],
    hdrs = ["random.h"],
)

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
        "//cs/math:constants",
        "@googletest//:gtest_main",
    ],
)
