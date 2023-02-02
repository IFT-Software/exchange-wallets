cc_binary(
  name = "main",
  srcs = ["main.cc"],
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
  ]
)