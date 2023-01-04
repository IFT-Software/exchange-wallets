cc_library(
  name = "cppzmq",
  srcs = [],
  hdrs = ["zmq.hpp", "zmq_addon.hpp"],
#   linkopts = ["-lzmq"],
  copts = ["-L", "external/libzmq/lib/libzmq.so"],
  deps = [
    "@libzmq"
  ],
  visibility = ["//visibility:public"],
)