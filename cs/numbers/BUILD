package(default_visibility = ["//visibility:public"])

cc_library(
    name = "clamp",
    hdrs = ["clamp.hh"],
)

cc_library(
    name = "in_range",
    hdrs = ["in_range.hh"],
)

cc_library(
    name = "map_value",
    hdrs = ["map_value.hh"],
)

cc_test(
    name = "map_value_test",
    srcs = ["map_value_test.cc"],
    deps = [
        ":map_value",
        "//cs/precision:floats",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "random",
    srcs = ["random.cc"],
    hdrs = ["random.hh"],
)
