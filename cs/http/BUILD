cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
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
        "//cs/sanity:ensure",
        "//cs/sanity:error",
    ],
)
