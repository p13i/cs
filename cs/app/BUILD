package(default_visibility = ["//visibility:public"])

load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_library(
    name = "fps_counter",
    hdrs = ["fps_counter.hh"],
)

# Exclude this library from the main build since the wasm_cc_binary
# below will compile the index.cc file with the emscripten SDK.
cc_library(
    name = "scene_animator",
    srcs = ["scene_animator.hh"],
    tags = ["manual"],
    deps = [
        "//cs/app/text/fonts:mono",
        "//cs/geo:point3",
        "//cs/geo:ray3",
        "//cs/geo:vector3",
        "//cs/linalg:transform",
        "//cs/numbers:map_value",
        "//cs/profiling:time_it",
        "//cs/renderer:rainbow",
        "//cs/renderer:scene_renderer",
        "//cs/sanity:ensure",
        "//cs/shapes:plane",
        "//cs/shapes:shape",
        "//cs/shapes:sphere",
    ],
)

cc_binary(
    name = "index",
    srcs = ["index.cc"],
    linkopts = [
        "-sASYNCIFY=1",
        "-sALLOW_MEMORY_GROWTH",
    ],
    # Exclude this library from the main build since the wasm_cc_binary
    # below will compile the index.cc file with the emscripten SDK.
    tags = ["manual"],
    deps = [
        "//cs/app:scene1",
        "//cs/app:scene_animator",
        "//cs/profiling:time_it",
        "//cs/renderer:film",
        "//cs/renderer:pixel",
    ],
)

wasm_cc_binary(
    name = "index.js",
    cc_target = ":index",
)

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
        "//cs/app:scene1",
        "//cs/app:scene_animator",
        "//cs/http:request",
        "//cs/http:response",
        "//cs/http:web_app",
        "//cs/net/json:object",
        "//cs/profiling:time_it",
        "//cs/renderer:film",
        "//cs/renderer:pixel",
        "//cs/result",
    ],
)

cc_library(
    name = "scene1",
    hdrs = ["scene1.hh"],
)
