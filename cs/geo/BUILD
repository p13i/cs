package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cross",
    hdrs = ["cross.hh"],
    deps = [
        ":point3",
        ":vector3",
    ],
)

cc_test(
    name = "cross_test",
    srcs = ["cross_test.cc"],
    deps = [
        ":cross",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "dist",
    srcs = ["dist.cc"],
    hdrs = ["dist.hh"],
    deps = [
        ":point3",
        ":vector3",
        "//cs/math:constants",
    ],
)

cc_test(
    name = "dist_test",
    srcs = ["dist_test.cc"],
    deps = [
        ":dist",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "dot",
    srcs = ["dot.cc"],
    hdrs = ["dot.hh"],
    deps = [
        ":point3",
        ":vector3",
        "//cs/math:constants",
        "//cs/sanity:ensure",
    ],
)

cc_test(
    name = "dot_test",
    srcs = ["dot_test.cc"],
    deps = [
        ":dot",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "point3",
    hdrs = ["point3.h"],
    deps = [
        "//cs/math:constants",
    ],
)

cc_test(
    name = "point3_test",
    srcs = ["point3_test.cc"],
    deps = [
        ":point3",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "vector3",
    hdrs = ["vector3.h"],
    deps = [
        ":point3",
        "//cs/precision:floats",
    ],
)

cc_test(
    name = "vector3_test",
    srcs = ["vector3_test.cc"],
    deps = [
        ":vector3",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "ray3",
    hdrs = ["ray3.h"],
    deps = [
        ":point3",
        ":vector3",
        "//cs/math:polar2cart",
    ],
)

cc_test(
    name = "ray3_test",
    srcs = ["ray3_test.cc"],
    deps = [
        ":ray3",
        "//cs/math:constants",
        "@googletest//:gtest_main",
    ],
)
