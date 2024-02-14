load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

new_git_repository(
    name = "googletest",
    build_file = "@//:gmock.BUILD",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    remote = "https://github.com/google/googletest",
)

git_repository(
    name = "emsdk",
    commit = "a896e3d066448b3530dbcaa48869fafefd738f57",
    remote = "https://github.com/emscripten-core/emsdk.git",
    strip_prefix = "bazel",
)

load("@emsdk//:deps.bzl", emsdk_deps = "deps")

emsdk_deps()

load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps(emscripten_version = "3.1.44")

load("@emsdk//:toolchains.bzl", "register_emscripten_toolchains")

register_emscripten_toolchains()
