#include "bitcoin/privkey.h"

#include <array>
#include <cstdint>
#include <stdexcept>

#include "bitcoin/crypto.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/signature.h"
#include "secp256k1.h"
#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "third_party/gmpxx/gmpxx.h"
#include "util/crypto.h"
#include "util/util.h"

namespace {
/* Global secp256k1_context object used for verification. */
secp256k1_context* secp256k1_context_verify = nullptr;
}  // namespace

PrivKey::PrivKey() : valid_(false) {}

PrivKey::PrivKey(uint8_t* begin, uint8_t* end) {
  if (end - begin != 32) {
    throw std::invalid_argument("private key is not ");
  }
  if (!Check(begin)) {
    throw std::invalid_argument("private key is 0 or greater than the secp256k1 curve order");
  }
  valid_ = true;
  std::copy(begin, end, priv_key_.begin());
}

PrivKey::PrivKey(std::array<uint8_t, 32>& priv_key) { PrivKey(priv_key.begin(), priv_key.end()); }

bool PrivKey::Check(uint8_t* priv_key) {
  return secp256k1_ec_seckey_verify(secp256k1_context_verify, priv_key);
}

bool PrivKey::IsValid() { return valid_; }

uint32_t PrivKey::size() const { return 32; }
const uint8_t* PrivKey::data() const { return priv_key_.begin(); }
const uint8_t* PrivKey::begin() const { return priv_key_.begin(); }
const uint8_t* PrivKey::end() const { return priv_key_.end(); }

const uint8_t& PrivKey::operator[](uint32_t pos) const { return priv_key_[pos]; }

std::string PrivKey::hex() const { return util::BytesToHex(priv_key_.begin(), size()); }
std::string PrivKey::bin() const { return util::BytesToBin(priv_key_.begin(), size()); }

PubKey PrivKey::GetPubKey() {
  std::array<uint8_t, PubKey::kCompressed> key;
  bitcoin::crypto::GeneratePubKey(priv_key_, key);
  return PubKey(key);
}

// TODO: return values
bool PrivKey::DeriveNormalChild(const uint32_t n_child, ChainCode& chain_code, uint8_t* child_priv,
                                uint8_t* child_chain) {
  mpz_class n("115792089237316195423570985008687907852837564279074904382605163141518161494337", 10);

  std::array<uint8_t, 37> res;
  PubKey pub_key = GetPubKey();

  std::copy(pub_key.begin(), pub_key.end(), res.begin());
  util::UInt32ToBytes(n_child, res.begin() + 33);

  std::array<uint8_t, 64> hmac_res;
  util::crypto::HMAC_SHA512(chain_code.begin(), 32, res.begin(), res.size(), hmac_res.begin());

  std::string first_32 = util::BytesToHex(hmac_res.begin(), 32);

  mpz_class first_32_i(first_32, 16);
  mpz_class old_priv_i(util::BytesToHex(priv_key_), 16);

  mpz_class new_priv_key = (first_32_i + old_priv_i) % n;

  std::vector<uint8_t> new_priv_key_vec;
  util::MpIntToBytes(new_priv_key, new_priv_key_vec);

  std::copy(new_priv_key_vec.begin(), new_priv_key_vec.end(), child_priv);
  std::copy(hmac_res.begin() + 32, hmac_res.end(), child_chain);
}

bool PrivKey::DeriveNormalChild(const uint32_t n_child, ChainCode& chain_code,
                                std::array<uint8_t, 32>& child_priv, ChainCode& child_chain) {
  DeriveNormalChild(n_child, chain_code, child_priv.begin(), child_chain.begin());
};

ExtPrivKey::ExtPrivKey() : valid_(false) {}

ExtPrivKey::ExtPrivKey(PrivKey& priv_key, ChainCode& chain_code) {
  if (!priv_key.IsValid()) {
    throw std::invalid_argument("invalid private key.");
  }
  valid_ = true;
  std::copy(priv_key.begin(), priv_key.end(), ext_priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), ext_priv_key_.begin() + 32);
};

ExtPrivKey::ExtPrivKey(std::array<uint8_t, 32>& priv_key, ChainCode& chain_code) {
  if (!Check(priv_key.begin())) {
    throw std::invalid_argument("private key is 0 or greater than the secp256k1 curve order.");
  }
  valid_ = true;
  std::copy(priv_key.begin(), priv_key.end(), ext_priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), ext_priv_key_.begin() + 32);
};

ExtPrivKey::ExtPrivKey(std::array<uint8_t, 64>& ext_priv_key) {
  if (!Check(ext_priv_key.begin())) {
    throw std::invalid_argument("private key is 0 or greater than the secp256k1 curve order.");
  }
  std::copy(ext_priv_key.begin(), ext_priv_key.end(), ext_priv_key_.begin());
};

uint32_t ExtPrivKey::size() const { return 64; }

const uint8_t* ExtPrivKey::data() const { return ext_priv_key_.begin(); }
const uint8_t* ExtPrivKey::begin() const { return ext_priv_key_.begin(); }
const uint8_t* ExtPrivKey::end() const { return ext_priv_key_.end(); }
const uint8_t& ExtPrivKey::operator[](uint32_t pos) const { return ext_priv_key_[pos]; }

// TODO: neden arrayi verince calismiyor?
std::string ExtPrivKey::hex() const { return util::BytesToHex(ext_priv_key_.begin(), 64); }
std::string ExtPrivKey::bin() const { return util::BytesToBin(ext_priv_key_.begin(), 64); }

bool ExtPrivKey::IsValid() { return valid_; }

bool ExtPrivKey::Check(uint8_t* priv_key) {
  return secp256k1_ec_seckey_verify(secp256k1_context_verify, priv_key);
}

PrivKey ExtPrivKey::GetPrivKey() {
  return PrivKey(ext_priv_key_.begin(), ext_priv_key_.begin() + 32);
}

ChainCode ExtPrivKey::GetChainCode() {
  std::array<uint8_t, 32> chain_code;
  std::copy(ext_priv_key_.begin() + 32, ext_priv_key_.end(), chain_code.begin());
  return chain_code;
}

bool ExtPrivKey::DeriveNormalChild(const uint32_t n_child, ExtPrivKey& child_ext_key) {
  PrivKey pk = GetPrivKey();
  ChainCode cc = GetChainCode();
  pk.DeriveNormalChild(n_child, cc, (uint8_t*)child_ext_key.begin(),
                       (uint8_t*)child_ext_key.begin() + 32);
};

ExtPrivKey ExtPrivKey::DeriveNormalChild(const uint32_t n_child) {
  ExtPrivKey ext_pkey;
  if (!DeriveNormalChild(n_child, ext_pkey)) {
    std::runtime_error("error while deriving child key");
  }
  return ext_pkey;
};