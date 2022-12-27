#ifndef LIB_SEED_H
#define LIB_SEED_H

#include <openssl/rand.h>
#include <stdexcept>
#include <bitset>
#include <climits>
#include <array>

#include "util.h"
#include "crypto.h"

class BIP39
{

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

  std::bitset<4> GenerateChecksum();
  std::string GenerateMnemonic();

public:
  BIP39();
  BIP39(std::string passphrase);
  std::bitset<128> GenerateEntropy();
  std::array<uint8_t, 64> GenerateSeed();
  std::array<uint8_t, 64> GenerateSeed(std::string passphrase);
  std::array<uint8_t, 64> GenerateSeedFromEntropy(std::bitset<128> entropy);

  std::bitset<128> GetEntropy();
  std::array<uint8_t, 64> GetSeed();
  std::bitset<4> GetChecksum();
  std::string GetMnemonic();

  std::string GetEntropyStr();
  std::string GetSeedStr();
  std::string GetChecksumStr();
};

#endif