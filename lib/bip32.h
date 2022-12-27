#ifndef LIB_BIP32_H
#define LIB_BIP32_H

#include <string>
#include <array>

#include "bip39.h"
#include "crypto.h"

class BIP32
{

private:
  static constexpr size_t kExtPrivKeySize = 64;
  static constexpr size_t kPrivKeySize = 32;
  static constexpr size_t kPubKeySize = 32;
  static constexpr size_t kChainCodeSize = 32;
  // static constexpr size_t k

  std::array<uint8_t, 64> m_ext_priv_key_;
  std::array<uint8_t, 32> m_priv_key_;
  std::array<uint8_t, 32> m_pub_key_;
  std::array<uint8_t, 32> chain_code_;

  std::string wif_;

  std::array<uint8_t, 4> GenerateChecksum(std::array<uint8_t, 34> &extended);

public:
  // generates master public and private keys from a random seed and
  BIP32();
  // generates master public and private keys from a given seed
  BIP32(std::array<uint8_t, 64> &seed);

  std::array<uint8_t, 32> GeneratePrivKey();

  std::array<uint8_t, 32> GeneratePrivKey(std::array<uint8_t, 64> &seed);
  std::string GenerateWIF(int net);
  std::string GenerateWIF(int net, std::array<uint8_t, 64> seed);

  // can be removed
  std::array<uint8_t, 32> GetPrivKey();
  std::string GetWIF();
  std::string GetPrivKeyStr();
  // std::array<uint8_t, 4> GetChecksum();
  // std::string GetChecksumStr();
};

#endif