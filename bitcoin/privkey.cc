#include "privkey.h"

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