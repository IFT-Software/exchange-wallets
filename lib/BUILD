cc_library(
  name = "lib",
  srcs = ["bip32.cc", "bip39.cc"],
  hdrs = ["bip32.h", "bip39.h"],
  data = ["wordlist.txt"],
  deps = [
    "//util",
    "@libsecp256k1//:libsecp256k1"
  ],
  visibility = ["//visibility:public"],
)