package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//cs/db:query_view",
        "//cs/db:table",
        "//cs/db/models:user",
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/net/json:serialize",
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_library(
    name = "query_view",
    hdrs = ["query_view.hh"],
    deps = [
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_library(
    name = "table",
    hdrs = ["table.hh"],
    deps = [
        "//cs/db:query_view",
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/result",
    ],
)

cc_library(
    name = "database",
    hdrs = ["database.hh"],
    deps = [
        "//cs/db:table",
        "//cs/result",
    ],
)

cc_library(
    name = "in_memory_database",
    srcs = ["in_memory_database.cc"],
    hdrs = ["in_memory_database.hh"],
    deps = [
        "//cs/db:database",
    ],
)
