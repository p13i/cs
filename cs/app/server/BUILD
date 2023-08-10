filegroup(
    name = "index_html",
    srcs = ["index.html", "index.js", "index.wasm"],
    visibility = ["//visibility:public"],
)

py_binary(
    name = "server",
    srcs = ["server.py"],
    data = [":index_html"],  # Replace with the path to your HTML file rule
)
