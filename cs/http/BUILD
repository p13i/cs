package(default_visibility = ["//visibility:public"])

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
