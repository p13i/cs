package(default_visibility = ["//visibility:public"])

cc_library(
    name = "matrix4x4",
    srcs = ["matrix4x4.cc"],
    hdrs = ["matrix4x4.hh"],
    deps = [
        "//cs/numbers:in_range",
        "//cs/sanity:ensure",
        "//cs/sanity:error",
        "//cs/string:format",
    ],
)

cc_library(
    name = "transform",
    srcs = ["transform.cc"],
    hdrs = ["transform.hh"],
    deps = [
        "//cs/geo:cross",
        "//cs/geo:vector3",
        "//cs/linalg:matrix4x4",
        "//cs/sanity:ensure",
    ],
)
