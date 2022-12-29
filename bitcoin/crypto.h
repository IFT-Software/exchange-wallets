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

namespace bitcoin {
namespace crypto {
namespace {

[[maybe_unused]] const std::array<uint8_t, 64> GenerateExtPrivKey(std::array<uint8_t, 64>& seed) {
  std::array<uint8_t, 64> extended_priv_key;
  const std::string key = "Bitcoin seed";

  util::crypto::HMAC_SHA512(key, seed.begin(), 64, extended_priv_key.begin());

  // std::copy(result.begin(), result.begin() + 32, m_priv_key_.begin());
  // std::copy(result.begin() + 32, result.end(), chain_code_.begin());

  return extended_priv_key;
}

[[maybe_unused]] const std::array<uint8_t, 32> GeneratePrivKey(std::array<uint8_t, 64>& seed) {
  std::array<uint8_t, 64> extended_prv_key = GenerateExtPrivKey(seed);

  std::array<uint8_t, 32> prv_key;
  std::copy(extended_prv_key.begin(), extended_prv_key.begin() + 32, prv_key.begin());

  return prv_key;
}

[[maybe_unused]] const std::array<uint8_t, 32> GenerateChainCode(std::array<uint8_t, 64>& seed) {
  std::array<uint8_t, 64> extended_prv_key = GenerateExtPrivKey(seed);

  std::array<uint8_t, 32> chain_code;
  std::copy(extended_prv_key.begin() + 32, extended_prv_key.end(), chain_code.begin());

  return chain_code;
}

[[maybe_unused]] const std::array<uint8_t, 4> GenerateChecksum(std::array<uint8_t, 34>& extended) {
  std::array<uint8_t, 32> res;
  std::array<uint8_t, 4> checksum;
  util::crypto::SHA256(extended.begin(), 1 + 32 + 1, res.begin());
  util::crypto::SHA256(res.begin(), res.size(), res.begin());

  std::copy(res.begin(), res.begin() + 4, checksum.begin());
  return checksum;
}

[[maybe_unused]] const std::string GenerateWIF(const int& net, std::array<uint8_t, 32>& priv_key) {
  uint8_t prefix;
  uint8_t suffix = {0x01};

  // net = 1 mainnet, net = 0 testnet
  if (net) {
    prefix = 0x80;
  } else {
    prefix = 0xEF;
  };

  std::array<uint8_t, 1 + 32 + 1> extended;
  extended[0] = prefix;
  std::copy(priv_key.begin(), priv_key.end(), extended.begin() + 1);
  extended[33] = suffix;

  std::array<uint8_t, 4> checksum = GenerateChecksum(extended);
  std::array<uint8_t, 34 + 4> extended_checksum;
  std::copy(extended.begin(), extended.end(), extended_checksum.begin());
  std::copy(checksum.begin(), checksum.end(), extended_checksum.begin() + extended.size());

  util::crypto::CodecMapping mapping(util::crypto::kAlphaMap, util::crypto::kBase58Map);
  return util::crypto::Base58Encode(
      std::vector<uint8_t>(extended_checksum.begin(), extended_checksum.end()), mapping);
}

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

[[maybe_unused]] const std::array<uint8_t, 33> GeneratePubKey(std::array<uint8_t, 32>& priv_key) {
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_pubkey pubkey;

  if (secp256k1_ec_seckey_verify(ctx, priv_key.begin())) {
    std::cout << "verified" << std::endl;
  }

  int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, priv_key.begin());
  assert(return_val);

  std::array<uint8_t, 33> pub_key;
  size_t size = pub_key.size();
  // std::array<uint8_t, 65> output;
  return_val =
      secp256k1_ec_pubkey_serialize(ctx, pub_key.begin(), &size, &pubkey, SECP256K1_EC_COMPRESSED);
  assert(return_val);

  return pub_key;
}

[[maybe_unused]] const std::array<uint8_t, 65> GeneratePubKeyUncomp(
    std::array<uint8_t, 32>& priv_key) {
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_pubkey pubkey;

  if (secp256k1_ec_seckey_verify(ctx, priv_key.begin())) {
    std::cout << "verified" << std::endl;
  }

  int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, priv_key.begin());
  assert(return_val);

  std::array<uint8_t, 65> pub_key;
  size_t size = pub_key.size();
  // std::array<uint8_t, 65> output;
  return_val = secp256k1_ec_pubkey_serialize(ctx, pub_key.begin(), &size, &pubkey,
                                             SECP256K1_EC_UNCOMPRESSED);
  assert(return_val);

  return pub_key;
}

[[maybe_unused]] const std::array<uint8_t, 20> GeneratePubKeyHash(
    std::array<uint8_t, 33>& pub_key) {
  std::array<uint8_t, 32> res_sha256;
  std::array<uint8_t, 20> res_ripemd160;
  util::crypto::SHA256(pub_key.begin(), pub_key.size(), res_sha256.begin());
  util::crypto::RIPEMD160(res_sha256.begin(), res_sha256.size(), res_ripemd160.begin());

  return res_ripemd160;
}

[[maybe_unused]] const std::array<uint8_t, 20> GeneratePubKeyHash(
    std::array<uint8_t, 65>& pub_key) {
  std::array<uint8_t, 32> res_sha256;
  std::array<uint8_t, 20> res_ripemd160;
  util::crypto::SHA256(pub_key.begin(), pub_key.size(), res_sha256.begin());
  util::crypto::RIPEMD160(res_sha256.begin(), res_sha256.size(), res_ripemd160.begin());

  return res_ripemd160;
}

}  // namespace
}  // namespace crypto
}  // namespace bitcoin

#endif