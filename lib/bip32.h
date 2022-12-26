#ifndef LIB_BIP32_H
#define LIB_BIP32_H

#include <string>

#include "bip39.h"
#include "crypto.h"

class BIP32
{

private:
  static constexpr size_t kExtPrivKeySize = 32;
  static constexpr size_t kPrivKeySize = 32;
  static constexpr size_t kPubKeySize = 32;
  static constexpr size_t kChainCodeSize = 32;

  uint8_t *m_ext_priv_key_;
  uint8_t *m_priv_key_;
  uint8_t *m_pub_key_;
  uint8_t *chain_code_;

  uint8_t *GeneratePrivateKey();

public:
  // generates master public and private keys from a random seed and
  BIP32();
  // generates master public and private keys from a given seed
  BIP32(uint8_t *seed);
  uint8_t *GeneratePrivKey();
  uint8_t *GeneratePrivKey(uint8_t *seed);
  uint8_t *GetPrivKey();
  std::string GetPrivKeyStr();
};

#endif