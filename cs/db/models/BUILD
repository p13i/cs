package(default_visibility = ["//visibility:public"])

py_binary(
    name = "codegen",
    srcs = ["codegen.py"],
)

cc_library(
    name = "user",
    hdrs = ["user.hh"],
    deps = [
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/net/json:serialize",
        "//cs/result",
    ],
)
