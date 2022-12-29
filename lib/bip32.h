#ifndef LIB_BIP32_H
#define LIB_BIP32_H

#include <secp256k1.h>

#include <array>
#include <cassert>
#include <iostream>
#include <string>

#include "lib/bip39.h"
#include "util/crypto.h"
#include "util/util.h"

class BIP32 {
 private:
  std::array<uint8_t, 64> m_ext_priv_key_;
  std::array<uint8_t, 32> m_priv_key_;
  std::array<uint8_t, 65> m_pub_key_unc;
  std::array<uint8_t, 33> m_pub_key_c;
  std::array<uint8_t, 32> chain_code_;
  std::array<uint8_t, 20> m_pub_key_hashed;

  std::string wif_;

  const std::array<uint8_t, 4> GenerateChecksum(std::array<uint8_t, 34>& extended);

 public:
  BIP32();
  BIP32(std::array<uint8_t, 64>& seed);

  /**
   * @brief Generates 32 bytes private key from a random seed and sets
   * m_priv_key_ to this key.
   *
   * @return std::array<uint8_t, 32>
   */
  const std::array<uint8_t, 32> GeneratePrivKey();

  /**
   * @brief Generates 32 bytes private key from the given seed and sets
   * m_priv_key_ to this key.
   *
   * @param seed used to generate the private key.
   * @return std::array<uint8_t, 32>
   */
  const std::array<uint8_t, 32> GeneratePrivKey(std::array<uint8_t, 64>& seed);

  /**
   * @brief Converts m_priv_key into "Wallet Import Format". Uses 01 as a suffix
   * Depending on the network, uses 0x80 or 0xEF as a prefix. Sets wif_ to the
   * return value of this function.
   *
   * @param net Indicates the network type. 1 is mainnet, 0 is testnet.
   * @return const std::string
   */
  const std::string GenerateWIF(const int& net);

  /**
   * @brief Converts priv_key into "Wallet Import Format". Uses 01 as a suffix
   * Depending on the network, uses 0x80 or 0xEF as a prefix.
   *
   * @param net Indicates the network type. 1 is mainnet, 0 is testnet.
   * @param priv_key The private key that is converted.
   * @return const std::string
   */
  const std::string GenerateWIF(const int& net, std::array<uint8_t, 32>& priv_key);

  const std::array<uint8_t, 33> GeneratePubKey();
  const std::array<uint8_t, 65> GeneratePubKeyUncomp();
  const std::array<uint8_t, 33> GeneratePubKey(std::array<uint8_t, 32>& priv_key);
  const std::array<uint8_t, 65> GeneratePubKeyUncomp(std::array<uint8_t, 32>& priv_key);

  const std::array<uint8_t, 20> GeneratePubKeyHash();
  const std::array<uint8_t, 20> GeneratePubKeyHash(std::array<uint8_t, 33>& pub_key);
  const std::array<uint8_t, 20> GeneratePubKeyHash(std::array<uint8_t, 65>& pub_key);

  const std::array<uint8_t, 32> GetPrivKey();
  const std::array<uint8_t, 33> GetPubKey();
  const std::array<uint8_t, 65> GetPubKeyUncomp();
  const std::array<uint8_t, 20> GetPubKeyHashed();

  const std::string GetWIF();
  const std::string GetPrivKeyStr();
  const std::string GetPubKeyStr();
  const std::string GetPubKeyUncompStr();
  const std::string GetPubKeyHashedStr();
};

#endif