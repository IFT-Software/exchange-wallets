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
    "@curl",
    "//util",
    "//third_party/cppzmq",
  ],
  linkstatic = True,
  visibility = ["//visibility:public"],
)