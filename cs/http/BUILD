cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":request",
        ":response",
        ":web_app",
        "//cs/app:scene_animator",
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
        "//cs/result",
        "//cs/sanity:ensure",
        "//cs/sanity:error",
    ],
)

cc_library(
    name = "request",
    srcs = ["request.cc"],
    hdrs = ["request.hh"],
    deps = [
        "//cs/result",
        "//cs/sanity:ensure",
        "//cs/sanity:error",
    ],
)

cc_library(
    name = "response",
    srcs = ["response.cc"],
    hdrs = ["response.hh"],
    deps = [
        ":status",
        "//cs/result",
        "//cs/sanity:ensure",
        "//cs/sanity:error",
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
