package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sphere",
    hdrs = ["sphere.hh"],
    deps = [
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
