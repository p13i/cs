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
