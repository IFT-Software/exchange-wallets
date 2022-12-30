cc_library(
  name = "bitcoin",
  srcs = ["pubkey.cc", "privkey.cc"],
  hdrs = ["crypto.h", "bip39.h", "pubkey.h", "privkey.h"],
  data = ["wordlist.txt"],
  deps = [
    "//util",
    "@libsecp256k1//:libsecp256k1",
    "@com_google_absl//absl/strings"
  ],
  visibility = ["//visibility:public"],
)