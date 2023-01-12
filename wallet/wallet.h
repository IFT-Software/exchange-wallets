#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "wallet/wallet_address.h"

class Wallet {
 private:
  std::array<uint8_t, 32> m_priv_key;
  std::array<uint8_t, 32> m_public_key;

  std::vector<WalletAddress> nodes_;

 public:
};

#endif
