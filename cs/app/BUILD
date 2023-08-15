package(default_visibility = ["//visibility:public"])

load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_library(
    name = "fps_counter",
    hdrs = ["fps_counter.h"],
)

cc_binary(
    name = "index",
    srcs = ["index.cc"],
    linkopts = [
        # "--oformat=html",
        # "-s MINIFY_HTML=0",
        "-sASYNCIFY=1",
    ],
    # Exclude this library from the main build since the wasm_cc_binary
    # below will compile the index.cc file with the emscripten SDK.
    tags = ["manual"],
    deps = [
        "//cs/renderer:rainbow",
        "//cs/renderer:scene_renderer",
        "//cs/renderer:sphere_renderer",
    ],
)

wasm_cc_binary(
    name = "index.js",
    cc_target = ":index",
)
