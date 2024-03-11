cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//cs/db:query_view",
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/net/json:serialize",
        "//cs/result",
    ],
)

cc_library(
    name = "query_view",
    hdrs = ["query_view.hh"],
    deps = [
        "//cs/result",
    ],
)
