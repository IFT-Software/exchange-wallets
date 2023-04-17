cc_library(
  name = "util",
  srcs = ["crypto.cc", "util.cc" ],
  hdrs = ["crypto.h", "util.h", "exceptions.h"],
  deps = [
    "@openssl",
    "//third_party/gmpxx",
    "@boost//:json",
    "@com_google_absl//absl/strings",
  ],
  visibility = ["//visibility:public"],
)