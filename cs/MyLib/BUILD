cc_library(
    name = "message",
    srcs = ["message.cpp"],
    hdrs = ["message.hpp"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "message_test",
    srcs = ["message_test.cpp"],
    deps = [
        ":message",
        "@googletest//:gtest_main",
    ],
)
