package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constants",
    hdrs = ["constants.h"],
)

cc_library(
    name = "in_range",
    hdrs = ["in_range.hh"],
)

cc_library(
    name = "map_value",
    hdrs = ["map_value.hh"],
)

cc_library(
    name = "matrix4x4",
    srcs = ["matrix4x4.cc"],
    hdrs = ["matrix4x4.hh"],
    deps = [
        "//cs/math:in_range",
        "//cs/sanity:ensure",
        "//cs/string:format",
    ],
)

cc_test(
    name = "map_value_test",
    srcs = ["map_value_test.cc"],
    deps = [
        ":constants",
        ":map_value",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "polar2cart",
    srcs = ["polar2cart.cc"],
    hdrs = ["polar2cart.hh"],
    deps = ["//cs/geo:point3"],
)

cc_library(
    name = "random",
    srcs = ["random.cc"],
    hdrs = ["random.h"],
)
