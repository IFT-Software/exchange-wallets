cc_library(
  name = "lib",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  data = ["wordlist.txt"],
  deps = [
    "@openssl//:openssl",
  ],
  visibility = ["//visibility:public"],
)