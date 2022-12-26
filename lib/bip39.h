#ifndef LIB_SEED_H
#define LIB_SEED_H

#include <openssl/rand.h>
#include <stdexcept>
#include <bitset>
#include <climits>

#include "util.h"
#include "crypto.h"

class BIP39
{

private:
  // random 16 bytes in hexadecimal format

  uint8_t bytes_[16];
  uint8_t seed_[64];

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
  uint8_t *GenerateSeed();
  uint8_t *GenerateSeed(std::string passphrase);
  uint8_t *GenerateSeedFromEntropy(std::bitset<128> entropy);

  std::bitset<128> GetEntropy();
  uint8_t *GetSeed();
  std::bitset<4> GetChecksum();
  std::string GetMnemonic();

  std::string GetEntropyStr();
  std::string GetSeedStr();
  std::string GetChecksumStr();
};

#endif