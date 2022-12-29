#ifndef BITCOIN_BIT39_H
#define BITCOIN_BIT39_H

// generates seed for the private and public keys using bip39 protocol
#include <openssl/rand.h>

#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "util/crypto.h"
#include "util/util.h"

namespace bitcoin {
namespace bip39 {
namespace {

template <size_t N>
[[maybe_unused]] bool GenerateRandBytes(std::array<uint8_t, N>& bytes) {
  int res = RAND_bytes(bytes.begin(), bytes.size());
  if (res != 1) {
    return false;
  }
  return true;
}

[[maybe_unused]] const std::bitset<128> GenerateEntropy(std::array<uint8_t, 16>& bytes) {
  std::string hex_bytes = util::BytesToHex(bytes);
  return std::bitset<128>(util::HexToBin(hex_bytes));
}

[[maybe_unused]] const std::bitset<4> GenerateChecksum(std::array<uint8_t, 16>& bytes,
                                                       const std::bitset<128>& entropy) {
  int checksum_size = (entropy).size() / 32;  // 1 bit checksum for every 32 bits of entropy

  std::array<uint8_t, 32> hex_sha256_bytes;
  util::crypto::SHA256(bytes.begin(), bytes.size(), hex_sha256_bytes.begin());

  std::string hex_sha256 = util::BytesToHex(hex_sha256_bytes);
  std::string bin_sha256 = util::HexToBin(hex_sha256);

  // std::string checksum_str = bin_sha256.substr(0, checksum_size);
  return std::bitset<4>(bin_sha256.substr(0, checksum_size));
}

[[maybe_unused]] const std::string GenerateMnemonic(const std::bitset<128>& entropy,
                                                    const std::bitset<4>& checksum) {
  std::string combined = entropy.to_string() + checksum.to_string();

  std::vector<uint64_t> int_list = util::UnpackBitStr(combined, 11, 0);

  std::vector<std::string> wordlist;
  std::vector<std::string> words;
  std::string mnemonic = "";

  wordlist = util::MakeWordList("bitcoin/wordlist.txt");
  for (uint64_t i : int_list) {
    std::string s = wordlist[i];
    if (!s.empty() && s[s.length() - 1] == '\n') {
      s.erase(s.length() - 1);
    }
    mnemonic += s + " ";
  }
  if (!mnemonic.empty()) {
    mnemonic.erase(mnemonic.length() - 1);
  }
  // std::cout << mnemonic << std::endl;
  return mnemonic;
}

[[maybe_unused]] const std::array<uint8_t, 64> GenerateSeed(std::string& mnemonic,
                                                            std::string& passphrase) {
  std::array<uint8_t, 64> seed;
  const std::string salt = "mnemonic" + passphrase;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic +
  // this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  util::crypto::PBKDF2_HMAC_SHA512(mnemonic.c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, seed.begin());
  // std::cout << "deneme5" << std::endl;
  return seed;
}

[[maybe_unused]] const std::array<uint8_t, 64> GenerateSeedFromEntropy(
    const std::bitset<128>& entropy, const std::string& passphrase) {
  std::array<uint8_t, 16> bytes;
  util::BinToBytes(entropy, bytes.begin());

  const std::bitset<4> checksum = GenerateChecksum(bytes, entropy);
  std::string mnemonic = GenerateMnemonic(entropy, checksum);

  const std::string salt = "mnemonic" + passphrase;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic +
  // this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  std::array<uint8_t, 64> seed;
  util::crypto::PBKDF2_HMAC_SHA512(mnemonic.c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, seed.begin());

  // std::string seed = util::BytesToString(seed_bytes, 64);
  // this->seed = seed_bytes;
  return seed;
}
}  // namespace
}  // namespace bip39
}  // namespace bitcoin

#endif