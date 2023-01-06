#include "bitcoin/bip39.h"

#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "absl/strings/str_join.h"
#include "openssl/rand.h"

#include "util/crypto.h"
#include "util/util.h"

namespace bitcoin {
namespace bip39 {

[[maybe_unused]] void GenerateEntropy(std::array<uint8_t, 16>& bytes, std::bitset<128>& res) {
  std::string hex_bytes = util::BytesToHex(bytes);
  res = std::bitset<128>(util::HexToBin(hex_bytes));
}

[[maybe_unused]] void GenerateChecksum(std::array<uint8_t, 16>& bytes,
                                       const std::bitset<128>& entropy, std::bitset<4>& res) {
  int checksum_size = (entropy).size() / 32;  // 1 bit checksum for every 32 bits of entropy

  std::array<uint8_t, 32> hex_sha256_bytes;
  util::crypto::SHA256(bytes.begin(), bytes.size(), hex_sha256_bytes.begin());

  std::string hex_sha256 = util::BytesToHex(hex_sha256_bytes);
  std::string bin_sha256 = util::HexToBin(hex_sha256);

  res = std::bitset<4>(bin_sha256.substr(0, checksum_size));
}

[[maybe_unused]] std::string GenerateMnemonic(const std::bitset<128>& entropy,
                                              const std::bitset<4>& checksum) {
  const std::string combined = entropy.to_string() + checksum.to_string();

  const std::vector<uint64_t> int_list = util::UnpackBitStr(combined, 11, 0);
  const std::vector<std::string> wordlist = util::MakeWordList("bitcoin/wordlist.txt");

  std::vector<std::string> mnemonic_list;
  for (uint64_t i : int_list) {
    mnemonic_list.push_back(wordlist[i]);
  }

  const std::string mnemonic = absl::StrJoin(mnemonic_list, " ");

  return mnemonic;
}

[[maybe_unused]] void GenerateSeed(const std::string& mnemonic, const std::string& passphrase,
                                   std::array<uint8_t, 64>& res) {
  const std::string salt = "mnemonic" + passphrase;
  const int iterations = 2048;
  const int keylength = 64;

  util::crypto::PBKDF2_HMAC_SHA512(mnemonic.c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, res.begin());
}

[[maybe_unused]] void GenerateSeedFromEntropy(const std::bitset<128>& entropy,
                                              const std::string& passphrase,
                                              std::array<uint8_t, 64>& res) {
  std::array<uint8_t, 16> bytes;
  util::BinToBytes(entropy, bytes);

  std::bitset<4> checksum;
  GenerateChecksum(bytes, entropy, checksum);

  const std::string mnemonic = GenerateMnemonic(entropy, checksum);
  const std::string salt = "mnemonic" + passphrase;

  const int iterations = 2048;
  const int keylength = 64;

  util::crypto::PBKDF2_HMAC_SHA512(mnemonic.c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, res.begin());
}

}  // namespace bip39
}  // namespace bitcoin