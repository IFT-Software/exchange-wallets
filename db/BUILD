cc_library(
  name = "db",
  srcs = [
    "db.cc", 
    "manager.cc", 
    "postgresql.cc",
    "managers/address_manager.cc",
    "managers/transaction_manager.cc",
    "managers/wallet_manager.cc",
    # "exceptions/execute_with_result.cc",
    # "exceptions/insert_error.cc"
    # "objects/address.cc",
    # "objects/wallet.cc"
  ],
  hdrs = [
    "db.h",
    "manager.h", 
    "postgresql.h",
    "managers/address_manager.h",
    "managers/transaction_manager.h",
    "managers/wallet_manager.h",
    # "exceptions/execute_with_result.h",
    # "exceptions/insert_error.h"
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