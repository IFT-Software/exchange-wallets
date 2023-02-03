cc_library(
  name = "net",
  srcs = [
    "comms.cc",
    "https.cc"
  ],
  hdrs = [
    "comms.h",
    "https.h"
  ],
  deps = [
    "//util",
    "//third_party/cppzmq",
    "@curl"
  ],
  visibility = ["//visibility:public"],
)