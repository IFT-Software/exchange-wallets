cc_binary(
  name = "main",
  srcs = ["main.cc"],
  linkopts = [
    "-lpq"
  ],
  deps = [
    "//bitcoin",
    "//db",
    "//socketio",
    "//db",
    "//third_party/cppzmq",
    "@boost//:multiprecision",
    "@libpqxx"
  ]
)