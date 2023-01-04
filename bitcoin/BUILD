cc_library(
  name = "bitcoin",
  srcs = ["address.cc", "crypto.cc", "pubkey.cc", "privkey.cc", "script.cc", "scriptpubkey.cc", "signature.cc"],
  hdrs = ["address.h", "bip39.h", "crypto.h", "pubkey.h", "privkey.h", "script.h", "scriptpubkey.h", "signature.h"],
  data = ["wordlist.txt"],
  deps = [
    "//util",
    "//libzmq:cppzmq",
    "@libsecp256k1//:libsecp256k1",
    "@com_google_absl//absl/strings"
  ],
  visibility = ["//visibility:public"],
)