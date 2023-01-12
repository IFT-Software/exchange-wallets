cc_library(
  name = "bitcoin",
  srcs = [
    "address.cc", 
    "bip39.cc", 
    "crypto.cc",
    "privkey.cc", 
    "pubkey.cc", 
    "script.cc", 
    "scriptpubkey.cc", 
    "signature.cc",
    "tx.cc",
  ],
  hdrs = [
    "address.h", 
    "bip39.h", 
    "crypto.h",
    "privkey.h",
    "pubkey.h",  
    "script.h", 
    "scriptpubkey.h", 
    "signature.h",
    "tx.h"
  ],
  data = ["wordlist.txt"],
  deps = [
    "//util",
    "@libsecp256k1",
    "@com_google_absl//absl/strings",
    "@boost//:multiprecision",
    "//third_party/cppzmq",
    "//third_party/gmpxx"
  ],
  visibility = ["//visibility:public"],
)