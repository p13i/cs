package(default_visibility = ["//visibility:public"])

cc_library(
    name = "film",
    hdrs = ["film.hh"],
    deps = [
        ":pixel",
    ],
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

cc_test(
    name = "rainbow_test",
    srcs = ["rainbow_test.cc"],
    deps = [
        ":rainbow",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "scene",
    srcs = ["scene.cc"],
    hdrs = ["scene.hh"],
    deps = [
        "//cs/geo:dist",
        "//cs/shapes:shape",
    ],
)

cc_library(
    name = "scene_renderer",
    srcs = ["scene_renderer.cc"],
    hdrs = ["scene_renderer.hh"],
    deps = [
        ":film",
        ":scene",
        "//cs/geo:dist",
        "//cs/geo:point3",
        "//cs/geo:ray3",
        "//cs/geo:vector3",
        "//cs/math:constants",
        "//cs/numbers:clamp",
        "//cs/numbers:map_value",
        "//cs/sanity:ensure",
        "//cs/shapes:sphere",
    ],
)

cc_test(
    name = "scene_renderer_test",
    srcs = ["scene_renderer_test.cc"],
    deps = [
        ":film",
        ":scene",
        ":scene_renderer",
        "//cs/geo:point3",
        "//cs/geo:ray3",
        "//cs/geo:vector3",
        "//cs/shapes:plane",
        "//cs/shapes:sphere",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "sphere_renderer",
    srcs = ["sphere_renderer.cc"],
    hdrs = ["sphere_renderer.hh"],
    deps = [
        ":film",
        "//cs/geo:dist",
        "//cs/geo:point3",
        "//cs/geo:ray3",
        "//cs/geo:vector3",
        "//cs/math:constants",
        "//cs/numbers:clamp",
        "//cs/numbers:map_value",
        "//cs/sanity:ensure",
        "//cs/shapes:sphere",
    ],
)

cc_test(
    name = "sphere_renderer_test",
    srcs = ["sphere_renderer_test.cc"],
    deps = [
        ":sphere_renderer",
        "@googletest//:gtest_main",
    ],
)
