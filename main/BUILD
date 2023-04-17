cc_binary(
  name = "main",
  srcs = ["main.cc", "global_vars.h", "thread_sync.h", "mempool_threads.h", "mempool_threads.cc"],
  linkopts = [
    "-lpq"
  ],
  deps = [
    "//bitcoin",
    "//db",
    "//net",
    "//third_party/cppzmq",
    "@boost//:multiprecision",
    "@libpqxx",
    "@com_google_absl//absl/strings",
  ],
  visibility = ["//visibility:public"],

)