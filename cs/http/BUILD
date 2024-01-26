cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":request",
        ":response",
        ":server",
        "//cs/app:scene_animator",
        "//cs/profiling:time_it",
        "//cs/renderer:film",
        "//cs/renderer:pixel",
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
        "//cs/sanity:ensure",
        "//cs/sanity:error",
    ],
)

cc_library(
    name = "request",
    srcs = ["request.cc"],
    hdrs = ["request.hh"],
    deps = [
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
        "//cs/sanity:ensure",
        "//cs/sanity:error",
    ],
)

cc_library(
    name = "status",
    hdrs = ["status.hh"],
)
