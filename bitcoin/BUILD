cc_library(
  name = "bitcoin",
  srcs = ["address.cc", "crypto.cc", "pubkey.cc", "privkey.cc"],
  hdrs = ["address.h", "bip39.h", "crypto.h", "pubkey.h", "privkey.h", "scriptpubkey.h"],
  data = ["wordlist.txt"],
  deps = [
    "//util",
    "@libsecp256k1//:libsecp256k1",
    "@com_google_absl//absl/strings"
  ],
  visibility = ["//visibility:public"],
)