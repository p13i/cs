package(default_visibility = ["//visibility:public"])

cc_library(
    name = "film",
    hdrs = ["film.hh"],
    deps = [":pixel"],
)

cc_library(
    name = "pixel",
    hdrs = ["pixel.hh"],
)

cc_library(
    name = "rainbow",
    srcs = ["rainbow.cc"],
    hdrs = ["rainbow.h"],
    deps = [":film"],
)
