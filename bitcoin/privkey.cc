#include "bitcoin/privkey.h"

#include <array>
#include <cstdint>

#include "bitcoin/crypto.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/signature.h"
#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "third_party/gmpxx/gmpxx.h"
#include "util/crypto.h"
#include "util/util.h"

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
  std::array<uint8_t, PubKey::kCompressed> key;
  bitcoin::crypto::GeneratePubKey(priv_key_, key);

  return PubKey(key);
}

bool PrivKey::DeriveNormalChild(uint32_t n_child, ChainCode& chain_code, uint8_t* child_priv,
                                uint8_t* child_chain) {
  mpz_class n("115792089237316195423570985008687907852837564279074904382605163141518161494337", 10);

  std::array<uint8_t, 37> res;
  PubKey pub_key = GetPubKey();
  std::cout << "GetPubKey(): " << util::BytesToHex(pub_key.begin(), 33) << std::endl;
  std::copy(pub_key.begin(), pub_key.end(), res.begin());
  // res[33] = 0x2c;
  util::UInt32ToBytes(n_child, res.begin() + 33);

  std::cout << "res: " << util::BytesToHex(res) << std::endl;
  std::cout << "chain code: " << util::BytesToHex(chain_code) << std::endl;

  std::array<uint8_t, 64> hmac_res;

  util::crypto::HMAC_SHA512(chain_code.begin(), 32, res.begin(), res.size(), hmac_res.begin());

  std::string first_32 = util::BytesToHex(hmac_res.begin(), 32);

  std::cout << "hmac first 32: " << first_32 << std::endl;

  mpz_class first_32_i(first_32, 16);

  // mpz_class first_32_i_10(first_32_i.get_str(10), 10);

  mpz_class old_priv_i(util::BytesToHex(priv_key_), 16);

  // mpz_class old_priv_i_10(old_priv_i.get_str(10), 10);

  mpz_class new_priv_key = (first_32_i + old_priv_i) % n;

  // std::cout << first_32_i.get_str()

  std::cout << "new priv key str: " << new_priv_key.get_str(16) << std::endl;

  // mpz_class new_priv_key_10 = (first_32_i_10 + old_priv_i_10) % n;
  // std::cout << "new priv key str10 : " << new_priv_key_10.get_str(16) << std::endl;

  std::vector<uint8_t> new_priv_key_vec;
  util::MpIntToBytes(new_priv_key, new_priv_key_vec);

  std::cout << "size: " << util::BytesToHex(new_priv_key_vec).length() << std::endl;

  std::copy(new_priv_key_vec.begin(), new_priv_key_vec.end(), child_priv);
  std::copy(hmac_res.begin() + 32, hmac_res.end(), child_chain);
}

bool PrivKey::DeriveNormalChild(uint32_t n_child, ChainCode& chain_code,
                                std::array<uint8_t, 32>& child_priv, ChainCode& child_chain) {
  DeriveNormalChild(n_child, chain_code, child_priv.begin(), child_chain.begin());
};

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(PrivKey priv_key, ChainCode chain_code) {
  std::copy(priv_key.begin(), priv_key.end(), priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), chain_code_.begin());
};

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(std::array<uint8_t, 32> priv_key, ChainCode chain_code) {
  std::copy(priv_key.begin(), priv_key.end(), priv_key_.begin());
  std::copy(chain_code.begin(), chain_code.end(), chain_code_.begin());
};

// todo: IsValid checks
ExtPrivKey::ExtPrivKey(std::array<uint8_t, 64> ext_priv_key) {
  std::copy(ext_priv_key.begin(), ext_priv_key.begin() + 32, priv_key_.begin());
  std::copy(ext_priv_key.begin() + 32, ext_priv_key.end(), chain_code_.begin());
};

bool ExtPrivKey::DeriveNormalChild(uint32_t n_child, std::array<uint8_t, 64>& child_ext_key) {
  PrivKey pk = PrivKey(priv_key_);
  pk.DeriveNormalChild(n_child, chain_code_, child_ext_key.begin(), child_ext_key.begin() + 32);
};