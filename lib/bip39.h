#ifndef LIB_BIP39_H
#define LIB_BIP39_H

#include <openssl/rand.h>

#include <array>
#include <bitset>
#include <climits>
#include <stdexcept>
#include <string>

#include "util/crypto.h"
#include "util/util.h"

class BIP39 {
 private:
  // random 16 bytes in hexadecimal format

  std::array<uint8_t, 16> bytes_;
  std::array<uint8_t, 64> seed_;

  // binary representation of bytes
  // why bitset?
  std::bitset<128> entropy_;

  // TODO checksum bitset?
  std::bitset<4> checksum_;
  std::string mnemonic_;
  std::string passphrase_;

  const std::bitset<4> GenerateChecksum();
  const std::string GenerateMnemonic();

 public:
  BIP39();
  BIP39(std::string passphrase);

  const std::bitset<128> GenerateEntropy();
  const std::array<uint8_t, 64> GenerateSeed();
  const std::array<uint8_t, 64> GenerateSeed(std::string& passphrase);
  const std::array<uint8_t, 64> GenerateSeedFromEntropy(std::bitset<128>& entropy);

  const std::bitset<128> GetEntropy();
  const std::bitset<4> GetChecksum();
  const std::string GetMnemonic();
  const std::array<uint8_t, 64> GetSeed();

  const std::string GetEntropyStr();
  const std::string GetSeedStr();
  const std::string GetChecksumStr();
};

#endif