#ifndef BITCOIN_BIP39_H
#define BITCOIN_BIP39_H

// generates seed for the private and public keys using bip39 protocol
#include <openssl/rand.h>

#include <array>
#include <bitset>
#include <cstdint>
#include <string>

namespace bitcoin {
namespace bip39 {

template <size_t N>
[[maybe_unused]] bool GenerateRandBytes(std::array<uint8_t, N>& bytes) {
  uint32_t res = RAND_bytes(bytes.begin(), bytes.size());
  if (res != 1) {
    return false;
  }
  return true;
}

[[maybe_unused]] void GenerateEntropy(std::array<uint8_t, 16>& bytes, std::bitset<128>& res);

[[maybe_unused]] void GenerateChecksum(std::array<uint8_t, 16>& bytes,
                                       const std::bitset<128>& entropy, std::bitset<4>& res);

[[maybe_unused]] std::string GenerateMnemonic(const std::bitset<128>& entropy,
                                              const std::bitset<4>& checksum);

[[maybe_unused]] void GenerateSeed(const std::string& mnemonic, const std::string& passphrase,
                                   std::array<uint8_t, 64>& res);

[[maybe_unused]] void GenerateSeedFromEntropy(const std::bitset<128>& entropy,
                                              const std::string& passphrase,
                                              std::array<uint8_t, 64>& res);

}  // namespace bip39
}  // namespace bitcoin

#endif