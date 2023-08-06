package(default_visibility = ["//visibility:public"])

cc_library(
    name = "point3",
    hdrs = ["point3.h"],
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
    deps = [":point3"],
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
    deps = [":point3", ":vector3"],
)

cc_test(
    name = "ray3_test",
    srcs = ["ray3_test.cc"],
    deps = [
        ":ray3",
        "@googletest//:gtest_main",
    ],
)
