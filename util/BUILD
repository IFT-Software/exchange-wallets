cc_library(
  name = "util",
  srcs = ["crypto.cc", "util.cc"],
  hdrs = ["crypto.h", "util.h"],
  linkopts = ["-lgmp", "-lgmpxx"],
  deps = [
    "@openssl//:openssl"
  ],
  visibility = ["//visibility:public"],
)