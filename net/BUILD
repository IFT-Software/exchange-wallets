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
    "@curl",
    "//util",
    "//third_party/cppzmq",
  ],
  visibility = ["//visibility:public"],
)