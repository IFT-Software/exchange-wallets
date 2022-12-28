cc_library(
  name = "lib",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  data = ["wordlist.txt"],
  deps = [
    "@openssl//:openssl",
    "@libsecp256k1//:libsecp256k1"
  ],
  visibility = ["//visibility:public"],
)