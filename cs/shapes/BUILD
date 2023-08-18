package(default_visibility = ["//visibility:public"])

cc_library(
    name = "shape",
    hdrs = ["shape.hh"],
    deps = [
        "//cs/geo:point3",
        "//cs/geo:ray3",
        "//cs/geo:vector3",
    ],
)

cc_library(
    name = "sphere",
    srcs = ["sphere.cc"],
    hdrs = ["sphere.hh"],
    deps = [
        ":shape",
        "//cs/geo:dot",
        "//cs/geo:ray3",
        "//cs/math:constants",
        "//cs/string:format",
    ],
)

cc_test(
    name = "sphere_test",
    srcs = ["sphere_test.cc"],
    deps = [
        ":sphere",
        "//cs/math:constants",
        "//cs/renderer:film",
        "//cs/renderer:pixel",
        "@googletest//:gtest_main",
    ],
)
