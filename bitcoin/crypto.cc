#include "bitcoin/crypto.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "scriptpubkey.h"
#include "secp256k1.h"

#include "bitcoin/address.h"
#include "bitcoin/bip39.h"
#include "util/crypto.h"
#include "util/util.h"

namespace bitcoin {
namespace crypto {

[[maybe_unused]] void GenerateExtPrivKey(const uint8_t* seed, uint8_t* extended_priv_key) {
  const std::string key = "Bitcoin seed";
  util::crypto::HMAC_SHA512(key, seed, 64, extended_priv_key);
}

[[maybe_unused]] void GenerateExtPrivKey(std::array<uint8_t, 64>& seed,
                                         std::array<uint8_t, 64>& extended_priv_key) {
  GenerateExtPrivKey(seed.begin(), extended_priv_key.begin());
}

[[maybe_unused]] void GeneratePrivKey(const uint8_t* seed, uint8_t* priv_key) {
  uint8_t extended_prv_key[64];
  GenerateExtPrivKey(seed, extended_prv_key);

  std::copy(extended_prv_key, extended_prv_key + 32, priv_key);
}

[[maybe_unused]] void GeneratePrivKey(std::array<uint8_t, 64>& seed,
                                      std::array<uint8_t, 32>& priv_key) {
  GeneratePrivKey(seed.begin(), priv_key.begin());
}

[[maybe_unused]] void GenerateChainCode(const uint8_t* seed, uint8_t* chain_code) {
  uint8_t extended_prv_key[64];
  GenerateExtPrivKey(seed, extended_prv_key);

  std::copy(extended_prv_key + 32, extended_prv_key + 64, chain_code);
}

[[maybe_unused]] void GenerateChainCode(std::array<uint8_t, 64>& seed,
                                        std::array<uint8_t, 32>& chain_code) {
  GenerateChainCode(seed.begin(), chain_code.begin());
}

[[maybe_unused]] void GenerateChecksum(const uint8_t* data, size_t len, uint8_t* res) {
  // std::array<uint8_t, 4> checksum;
  std::array<uint8_t, 32> res_;

  util::crypto::SHA256(data, len, res_.begin());
  util::crypto::SHA256(res_.begin(), 32, res_.begin());

  std::copy(res_.begin(), res_.begin() + 4, res);

  // std::cout << "data: " << util::BytesToHex(data, len) << std::endl;
  // std::cout << "checksum: " << util::BytesToHex(res, 4) << std::endl;
}

[[maybe_unused]] std::string GenerateWIF(const int net, const uint8_t* priv_key) {
  // net = 1 mainnet, net = 0 testnet
  uint8_t prefix = (net) ? 0x80 : 0xEF;
  uint8_t suffix = 0x01;

  std::array<uint8_t, 1 + 32 + 1> extended;
  extended[0] = prefix;
  std::copy(priv_key, priv_key + 32, extended.begin() + 1);
  extended[33] = suffix;

  std::array<uint8_t, 4> checksum;
  GenerateChecksum(extended, checksum);
  std::array<uint8_t, 34 + 4> extended_checksum;
  std::copy(extended.begin(), extended.end(), extended_checksum.begin());
  std::copy(checksum.begin(), checksum.end(), extended_checksum.begin() + extended.size());

  util::crypto::CodecMapping mapping(util::crypto::kAlphaMap, util::crypto::kBase58Map);
  return util::crypto::Base58Encode(
      std::vector<uint8_t>(extended_checksum.begin(), extended_checksum.end()), mapping);
}

[[maybe_unused]] std::string GenerateWIF(const int net, std::array<uint8_t, 32>& priv_key) {
  return GenerateWIF(net, priv_key.begin());
}

[[maybe_unused]] void GeneratePubKey(std::array<uint8_t, 32>& priv_key,
                                     std::array<uint8_t, 33>& pub_key) {
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_pubkey pubkey;

  // if (secp256k1_ec_seckey_verify(ctx, priv_key.begin())) {
  //   std::cout << "verified" << std::endl;
  // }

  uint32_t return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, priv_key.begin());
  assert(return_val);

  size_t size = pub_key.size();

  return_val =
      secp256k1_ec_pubkey_serialize(ctx, pub_key.begin(), &size, &pubkey, SECP256K1_EC_COMPRESSED);
  assert(return_val);
}

[[maybe_unused]] void GeneratePubKeyUncomp(std::array<uint8_t, 32>& priv_key,
                                           std::array<uint8_t, 65>& pub_key) {
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_pubkey pubkey;

  int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, priv_key.begin());
  assert(return_val);

  size_t size = pub_key.size();

  return_val = secp256k1_ec_pubkey_serialize(ctx, pub_key.begin(), &size, &pubkey,
                                             SECP256K1_EC_UNCOMPRESSED);
  assert(return_val);
}

[[maybe_unused]] void GeneratePubKeyHash(const uint8_t* pub_key, size_t len, uint8_t* res) {
  if ((len != 33 && len != 65)) {
    // TODO: custom exceptions
    throw std::invalid_argument("the length of the public key is invalid");
  };

  std::array<uint8_t, 32> res_sha256;
  util::crypto::SHA256(pub_key, len, res_sha256.begin());
  util::crypto::RIPEMD160(res_sha256.begin(), res_sha256.size(), res);
}

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 33>& pub_key,
                                         std::array<uint8_t, 20>& res) {
  GeneratePubKeyHash(pub_key.begin(), pub_key.size(), res.begin());
}

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 65>& pub_key,
                                         std::array<uint8_t, 20>& res) {
  GeneratePubKeyHash(pub_key.begin(), pub_key.size(), res.begin());
}

// TODO: write tests
std::string GenerateAddressFromPubkey(PubKey& pub_key, OutputType& addr_type) {
  switch (addr_type) {
    case OutputType::P2PKH: {
      std::array<uint8_t, 25> res;
      std::array<uint8_t, 20> pub_key_hash;
      std::array<uint8_t, 21> pkey_hash_checksum;
      pub_key_hash = pub_key.GetHash160();

      // TODO: make this a variable (based on test/mainnet)
      pkey_hash_checksum[0] = {0x00};
      std::copy(pub_key_hash.begin(), pub_key_hash.end(), pkey_hash_checksum.begin() + 1);

      bitcoin::crypto::GenerateChecksum(pkey_hash_checksum.begin(), pkey_hash_checksum.size(),
                                        res.begin() + 21);
      // res[0] = {0x00};
      std::copy(pkey_hash_checksum.begin(), pkey_hash_checksum.end(), res.begin());
      util::crypto::CodecMapping mapping(util::crypto::kAlphaMap, util::crypto::kBase58Map);

      return util::crypto::Base58Encode(std::vector<uint8_t>(res.begin(), res.end()), mapping);
    }
    case OutputType::P2SH:
      // TODO: custom exceptions
      throw std::invalid_argument("can't produce p2sh address from pubkey");
    default:
      // todo: custom exceptions
      throw std::invalid_argument("invalid address type");
  }
}

// Child Key Derivation (CKD) Functions

}  // namespace crypto
}  // namespace bitcoin