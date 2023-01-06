#include "bitcoin/pubkey.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "secp256k1.h"

#include "bitcoin/address.h"
#include "util/util.h"

namespace {
/* Global secp256k1_context object used for verification. */
secp256k1_context* secp256k1_context_verify = nullptr;
}  // namespace

PubKey::PubKey() { Invalidate(); }

uint32_t PubKey::GetSize(uint8_t pubkey_first_byte) const {
  if (pubkey_first_byte == 0x02 || pubkey_first_byte == 0x03) {
    return PubKey::Size::kCompressed;
  }
  if (pubkey_first_byte == 0x04) {
    return PubKey::Size::kUncompressed;
  }
  return 0;
}

void PubKey::Invalidate() { pub_key_[0] = 0xff; }

bool PubKey::IsCompressed() { return GetSize(pub_key_[0]) == PubKey::Size::kCompressed; }

bool PubKey::IsUncompressed() { return GetSize(pub_key_[0]) == PubKey::Size::kUncompressed; }

bool PubKey::IsValid() {
  if (size() <= 0) return false;
  secp256k1_pubkey pubkey;
  return secp256k1_ec_pubkey_parse(secp256k1_context_verify, &pubkey, pub_key_.begin(), size());
};

uint32_t PubKey::size() const { return GetSize(pub_key_[0]); }

const uint8_t* PubKey::data() const { return pub_key_.begin(); }
const uint8_t* PubKey::begin() const { return pub_key_.begin(); }
const uint8_t* PubKey::end() const { return pub_key_.begin() + size(); }
const uint8_t& PubKey::operator[](uint32_t pos) const { return pub_key_[pos]; }

std::string PubKey::hex() const { return util::BytesToHex(pub_key_.begin(), GetSize(pub_key_[0])); }
std::string PubKey::bin() const { return util::BytesToBin(pub_key_.begin(), GetSize(pub_key_[0])); }

// TODO: necessary?
std::array<uint8_t, 20> PubKey::GetHash160() {
  std::array<uint8_t, 20> res;
  bitcoin::crypto::GeneratePubKeyHash(pub_key_.begin(), GetSize(pub_key_[0]), res.begin());
  return res;
}
