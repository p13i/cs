cc_library(
    name = "message",
    hdrs = ["message.hpp"],
    srcs = ["message.cpp"],
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
