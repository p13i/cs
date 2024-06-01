package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
        "//cs/app:scene1",
        "//cs/app:scene_animator",
        "//cs/db:table",
        "//cs/db/models:user",
        "//cs/net/http:request",
        "//cs/net/http:response",
        "//cs/net/http:web_app",
        "//cs/net/json:object",
        "//cs/net/json:parsers",
        "//cs/net/json:serialize",
        "//cs/profiling:time_it",
        "//cs/renderer:film",
        "//cs/renderer:pixel",
        "//cs/result",
    ],
)

cc_library(
    name = "server",
    srcs = ["server.cc"],
    hdrs = ["server.hh"],
    deps = [
        ":request",
        ":response",
        "//cs/profiling:time_it",
        "//cs/random:uuid",
        "//cs/result",
    ],
)

cc_library(
    name = "parsers",
    srcs = ["parsers.cc"],
    hdrs = ["parsers.hh"],
    deps = [
        "//cs/result",
    ],
)

cc_library(
    name = "request",
    srcs = ["request.cc"],
    hdrs = ["request.hh"],
    deps = [
        "//cs/net/http:parsers",
        "//cs/result",
    ],
)

cc_library(
    name = "response",
    srcs = ["response.cc"],
    hdrs = ["response.hh"],
    deps = [
        ":status",
        "//cs/net/http:parsers",
        "//cs/result",
        "//cs/string:format",
    ],
)

cc_library(
    name = "status",
    hdrs = ["status.hh"],
)

cc_library(
    name = "web_app",
    hdrs = [
        "web_app.hh",
    ],
    deps = [
        ":request",
        ":response",
        ":server",
        "//cs/result",
    ],
)
