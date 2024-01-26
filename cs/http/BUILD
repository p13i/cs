cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":request",
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
        "//cs/sanity:ensure",
        "//cs/profiling:time_it",
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
