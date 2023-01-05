#include "privkey.h"

#include <cstdint>

#include "boost/multiprecision/gmp.hpp"
#include "crypto.h"

// todo: IsValid checks
PrivKey::PrivKey(std::array<uint8_t, 32>& priv_key) {
  std::copy(priv_key.begin(), priv_key.end(), priv_key_.begin());
}

uint32_t PrivKey::size() const { return 32; }
const uint8_t* PrivKey::data() const { return priv_key_.begin(); }
const uint8_t* PrivKey::begin() const { return priv_key_.begin(); }
const uint8_t* PrivKey::end() const { return priv_key_.end(); }

const uint8_t& PrivKey::operator[](uint32_t pos) const { return priv_key_[pos]; }

std::string PrivKey::hex() const { return util::BytesToHex(priv_key_.begin(), size()); }
std::string PrivKey::bin() const { return util::BytesToBin(priv_key_.begin(), size()); }

PubKey PrivKey::GetPubKey() {
  std::array<uint8_t, PubKey::kCompressed> key = bitcoin::crypto::GeneratePubKey(priv_key_);
  return PubKey(key);
}

namespace mpl = boost::multiprecision;

bool PrivKey::DeriveNormalChild(std::array<uint8_t, 32>& child_priv, ChainCode& child_chain,
                                uint32_t n_child, ChainCode& chain_code) {
  mpl::mpz_int n = 115792089237316195423570985008687907852837564279074904382605163141518161494337;

  std::array<uint8_t, 37> res;
  PubKey pub_key = GetPubKey();
  std::copy(pub_key.begin(), pub_key.end(), res.begin());
  util::UInt32ToBytes(n_child, res.begin() + 33);

  std::array<uint8_t, 64> hmac_res;

  util::crypto::HMAC_SHA512(util::BytesToHex(chain_code), res.begin(), res.size(),
                            hmac_res.begin());
};

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(PrivKey priv_key, ChainCode chain_code) {
  std::copy(priv_key.begin(), priv_key.end(), priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), chain_code_.begin());
}

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(std::array<uint8_t, 32> priv_key, ChainCode chain_code) {
  std::copy(priv_key.begin(), priv_key.end(), priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), chain_code_.begin());
}

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(std::array<uint8_t, 64> ext_priv_key) {
  std::copy(ext_priv_key.begin(), ext_priv_key.begin() + 32, priv_key_.begin());
  std::copy(ext_priv_key.begin() + 32, ext_priv_key.end(), chain_code_.begin());
}