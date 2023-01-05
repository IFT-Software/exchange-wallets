#ifndef BITCOIN_CRYPTO_H
#define BITCOIN_CRYPTO_H

#include <secp256k1.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "bitcoin/bip39.h"
#include "util/crypto.h"
#include "util/util.h"

class PubKey;
class Script;
enum class AddrType : uint16_t;

namespace bitcoin {
namespace crypto {

[[maybe_unused]] void GenerateExtPrivKey(const uint8_t* seed, uint8_t* res);
[[maybe_unused]] void GenerateExtPrivKey(std::array<uint8_t, 64>& seed,
                                         std::array<uint8_t, 64>& extended_priv_key);

[[maybe_unused]] void GeneratePrivKey(const uint8_t* seed, uint8_t* priv_key);
[[maybe_unused]] void GeneratePrivKey(std::array<uint8_t, 64>& seed,
                                      std::array<uint8_t, 32>& priv_key);

[[maybe_unused]] void GenerateChainCode(const uint8_t* seed, uint8_t* chain_code);
[[maybe_unused]] void GenerateChainCode(std::array<uint8_t, 64>& seed,
                                        std::array<uint8_t, 32>& chain_code);

[[maybe_unused]] void GenerateChecksum(const uint8_t* data, size_t len, uint8_t* res);

template <size_t N>
[[maybe_unused]] void GenerateChecksum(std::array<uint8_t, N>& data, std::array<uint8_t, 4>& res) {
  GenerateChecksum(data.begin(), data.size(), res.begin());
}

[[maybe_unused]] std::string GenerateWIF(const int net, const uint8_t* priv_key);
[[maybe_unused]] std::string GenerateWIF(const int net, std::array<uint8_t, 32>& priv_key);

// const std::array<uint8_t, 33> GeneratePubKey() {
//   m_pub_key_c = GeneratePubKey(m_priv_key_);
//   m_pub_key_unc = GeneratePubKeyUncomp(m_priv_key_);
//   return m_pub_key_c;
// }

// const std::array<uint8_t, 65> BIP32::GeneratePubKeyUncomp() {
//   m_pub_key_c = GeneratePubKey(m_priv_key_);
//   m_pub_key_unc = GeneratePubKeyUncomp(m_priv_key_);
//   return m_pub_key_unc;
// }

// TODO: make return type void
[[maybe_unused]] std::array<uint8_t, 33> GeneratePubKey(std::array<uint8_t, 32>& priv_key);

// TODO: make return type void
[[maybe_unused]] std::array<uint8_t, 65> GeneratePubKeyUncomp(std::array<uint8_t, 32>& priv_key);

[[maybe_unused]] void GeneratePubKeyHash(uint8_t* pub_key, size_t len, uint8_t* res);

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 33>& pub_key,
                                         std::array<uint8_t, 20>& res);

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 65>& pub_key,
                                         std::array<uint8_t, 20>& res);

// // used for p2pkh
[[maybe_unused]] std::string GenerateAddressFromPubkey(PubKey& pub_key, AddrType& addr_type);
// std::string GenerateAddressFromPubkey(std::array<uint8_t, 33>& pub_key);
// std::string GenerateAddressFromPubkey(std::array<uint8_t, 65> pub_key);

// // used for p2sh
// std::string GenerateAddressFromScript(Script& script);

// Child Key Derivation (CKD) functions

}  // namespace crypto
}  // namespace bitcoin

#endif