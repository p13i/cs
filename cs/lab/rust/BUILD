load("@rules_rust//rust:defs.bzl", "rust_binary", "rust_library")

package(default_visibility = ["//visibility:public"])

rust_library(
    name = "hello_lib",
    srcs = ["lib.rs"],
)

rust_binary(
    name = "hello_world",
    srcs = ["main.rs"],
    deps = [":hello_lib"],
)
