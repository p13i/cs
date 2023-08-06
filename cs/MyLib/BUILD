cc_library(
    name = "message",
    srcs = ["message.cc"],
    hdrs = ["message.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "message_test",
    srcs = ["message_test.cc"],
    deps = [
        ":message",
        "@googletest//:gtest_main",
    ],
)
