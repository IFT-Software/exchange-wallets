#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "wallet/wallet_address.h"

class Wallet {
 private:
  std::string seed_;

  std::array<uint8_t, 32> master_priv_key_;
  std::array<uint8_t, 32> master_public_key_;

  std::vector<WalletAddress> nodes_;

 public:
};

#endif
