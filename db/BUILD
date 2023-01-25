cc_library(
  name = "db",
  srcs = [
    "db.cc", 
    "manager.cc", 
    "postgresql.cc",
    "managers/address_manager.cc",
    "managers/wallet_manager.cc",
    # "objects/address.cc",
    # "objects/wallet.cc"
  ],
  hdrs = [
    "db.h",
    "manager.h", 
    "postgresql.h",
    "managers/address_manager.h",
    "managers/wallet_manager.h",
    # "objects/address.h",
    # "objects/wallet.h"

  ],
  deps = [
    "//util",
    "@libpqxx",
    "@boost//:json",
    "@com_google_absl//absl/strings",
  ],
  visibility = ["//visibility:public"]
)