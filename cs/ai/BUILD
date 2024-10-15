package(default_visibility = ["//visibility:public"])

cc_library(
    name = "neural_net",
    srcs = ["neural_net.cc"],
    hdrs = ["neural_net.hh"],
    deps = [
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_test(
    name = "neural_net_test",
    srcs = ["neural_net_test.cc"],
    copts = ["-Wno-format-security"],
    deps = [
        ":neural_net",
        "@googletest//:gtest_main",
    ],
)
