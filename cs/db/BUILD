package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//cs/db:query_view",
        "//cs/db:table",
        "//cs/db/models:user",
    ],
)

cc_library(
    name = "query_view",
    hdrs = ["query_view.hh"],
    deps = [
        "//cs/result",
    ],
)

cc_library(
    name = "table",
    hdrs = ["table.hh"],
    deps = [
        "//cs/db:query_view",
        "//cs/result",
    ],
)
