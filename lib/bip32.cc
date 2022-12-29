#include "bip32.h"

#include <cstddef>

#include "util.h"

BIP32::BIP32() { GeneratePrivKey(); }
BIP32::BIP32(std::array<uint8_t, 64>& seed) { GeneratePrivKey(seed); }

const std::array<uint8_t, 32> BIP32::GetPrivKey() { return m_priv_key_; }
const std::string BIP32::GetWIF() { return wif_; }
const std::array<uint8_t, 33> BIP32::GetPubKey() { return m_pub_key_c; }
const std::array<uint8_t, 65> BIP32::GetPubKeyUncomp() { return m_pub_key_unc; }
const std::array<uint8_t, 20> BIP32::GetPubKeyHashed() { return m_pub_key_hashed; }

const std::string BIP32::GetPrivKeyStr() { return Util::BytesToHex(m_priv_key_); }
const std::string BIP32::GetPubKeyStr() { return Util::BytesToHex(m_pub_key_c); }
const std::string BIP32::GetPubKeyUncompStr() { return Util::BytesToHex(m_pub_key_unc); }
const std::string BIP32::GetPubKeyHashedStr() { return Util::BytesToHex(m_pub_key_hashed); }

const std::array<uint8_t, 4> BIP32::GenerateChecksum(std::array<uint8_t, 34>& extended) {
  std::array<uint8_t, 32> res;
  std::array<uint8_t, 4> checksum;
  Crypto::SHA256(extended.begin(), 1 + 32 + 1, res.begin());
  Crypto::SHA256(res.begin(), res.size(), res.begin());

  std::copy(res.begin(), res.begin() + 4, checksum.begin());
  return checksum;
}

const std::array<uint8_t, 32> BIP32::GeneratePrivKey() {
  BIP39* seed_generator = new BIP39();
  const std::string key = "Bitcoin seed";
  std::array<uint8_t, 64> data = seed_generator->GetSeed();

  std::array<uint8_t, 64> result;
  Crypto::HMAC_SHA512(key, data.begin(), data.size(), result.begin());

  std::copy(result.begin(), result.begin() + 32, m_priv_key_.begin());
  std::copy(result.begin() + 32, result.end(), chain_code_.begin());

  return m_priv_key_;
}

const std::array<uint8_t, 32> BIP32::GeneratePrivKey(std::array<uint8_t, 64>& seed) {
  std::string key = "Bitcoin seed";

  std::array<uint8_t, 64> result;
  Crypto::HMAC_SHA512(key, seed.begin(), 64, result.begin());

  std::copy(result.begin(), result.begin() + 32, m_priv_key_.begin());
  std::copy(result.begin() + 32, result.end(), chain_code_.begin());

  return m_priv_key_;
}

const std::string BIP32::GenerateWIF(const int& net) {
  wif_ = GenerateWIF(net, m_priv_key_);
  return wif_;
}

const std::string BIP32::GenerateWIF(const int& net, std::array<uint8_t, 32>& priv_key) {
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

  Crypto::CodecMapping mapping(Crypto::kAlphaMap, Crypto::kBase58Map);
  return Crypto::Base58Encode(
      std::vector<uint8_t>(extended_checksum.begin(), extended_checksum.end()), mapping);
}

const std::array<uint8_t, 33> BIP32::GeneratePubKey() {
  m_pub_key_c = GeneratePubKey(m_priv_key_);
  m_pub_key_unc = GeneratePubKeyUncomp(m_priv_key_);
  return m_pub_key_c;
}

const std::array<uint8_t, 65> BIP32::GeneratePubKeyUncomp() {
  m_pub_key_c = GeneratePubKey(m_priv_key_);
  m_pub_key_unc = GeneratePubKeyUncomp(m_priv_key_);
  return m_pub_key_unc;
}

const std::array<uint8_t, 33> BIP32::GeneratePubKey(std::array<uint8_t, 32>& priv_key) {
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

const std::array<uint8_t, 65> BIP32::GeneratePubKeyUncomp(std::array<uint8_t, 32>& priv_key) {
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

const std::array<uint8_t, 20> BIP32::GeneratePubKeyHash() {
  m_pub_key_hashed = GeneratePubKeyHash(m_pub_key_c);
  return m_pub_key_hashed;
}

const std::array<uint8_t, 20> BIP32::GeneratePubKeyHash(std::array<uint8_t, 33>& pub_key) {
  std::array<uint8_t, 32> res_sha256;
  std::array<uint8_t, 20> res_ripemd160;
  Crypto::SHA256(pub_key.begin(), pub_key.size(), res_sha256.begin());
  Crypto::RIPEMD160(res_sha256.begin(), res_sha256.size(), res_ripemd160.begin());

  return res_ripemd160;
}

const std::array<uint8_t, 20> BIP32::GeneratePubKeyHash(std::array<uint8_t, 65>& pub_key) {
  std::array<uint8_t, 32> res_sha256;
  std::array<uint8_t, 20> res_ripemd160;
  Crypto::SHA256(pub_key.begin(), pub_key.size(), res_sha256.begin());
  Crypto::RIPEMD160(res_sha256.begin(), res_sha256.size(), res_ripemd160.begin());

  return res_ripemd160;
}
