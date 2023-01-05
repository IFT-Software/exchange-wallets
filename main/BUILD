cc_binary(
  name = "main",
  srcs = ["main.cc"],
  linkopts = ["-lgmp", "-lgmpxx"],
  deps = [
    "//bitcoin",
    "//third_party/cppzmq",
    "//comms",
    "@boost//:multiprecision"
  ]
)