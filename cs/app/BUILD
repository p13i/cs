cc_library(
    name = "fps_counter",
    hdrs = ["fps_counter.h"],
)

package(default_visibility = ["//visibility:public"])

load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_binary(
    name = "index",
    srcs = ["hello_world.cc"],
    linkopts = [
        "--oformat=html",
        "-s MINIFY_HTML=0",
    ],
)

wasm_cc_binary(
    name = "hello_world_wasm",
    cc_target = ":index",
)
