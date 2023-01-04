cc_binary(
  name = "main",
  srcs = ["main.cc"],
  linkopts = ["-lgmp"],
  deps = [
    "//bitcoin",
    "//third_party/cppzmq",
    "//comms",
    "@boost//:multiprecision"
  ]
)