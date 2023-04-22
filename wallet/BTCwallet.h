#ifndef WALLET_BTCWALLET_H
#define WALLET_BTCWALLET_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "bitcoin/address.h"
#include "bitcoin/scriptpubkey.h"
#include "util/exceptions.h"
#include "wallet/wallet_address.h"

typedef std::string DerivationPath;

// TODO: add other wallet types
enum WalletType { DEPOSIT, WITHDRAWAL };

class BTCWallet {
 private:
  std::array<uint8_t, 64> seed_;

  std::array<uint8_t, 32> master_priv_key_;
  std::array<uint8_t, 32> master_public_key_;

  DerivationPath last_path_;

 public:
  /**
   * @brief Constructor for a BTC wallet.
   *
   * @param seed
   */
  BTCWallet(std::array<uint8_t, 64> seed);

  /**
   * @brief Generates a new address for @param user_id using the last used derivation path + 1.
   * Returns an address object with user_id_ field set to user_id.
   *
   * @param user_id
   * @return std::unique_ptr<Address>
   */
  std::unique_ptr<Address> GenerateAddress(uint32_t user_id);

  /**
   * @brief Records this wallet to the database. Throws @exception ExistingWalletException if a
   * wallet with this seed exits.
   */
  void RecordDatabase();

  /**
   * @brief Updates the "last path" field of this wallet in the database. Throws @exception
   * NotExistingWalletException if a wallet associated with this seed is not present in the
   * database.
   */
  void UpdateDatabase();
};

#endif
