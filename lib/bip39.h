#ifndef LIB_SEED_H
#define LIB_SEED_H

#include <openssl/rand.h>
#include <stdexcept>
#include <bitset>
#include <climits>

#include "util.h"
#include "crypto.h"

class SeedGenerator {

  private:

    //random 16 bytes in hexadecimal format
    uint8_t bytes[16];

    //binary representation of bytes
    std::bitset<128> entropy;

    //TODO checksum bitset?
    std::string checksum;
    std::string mnemonic;
    std::string passphrase;
    std::string seed;

    std::bitset<128> GenerateEntropy();
    std::string GenerateChecksum();
    std::string GenerateMnemonic();
    

  public:
    SeedGenerator();
    SeedGenerator(std::string passphrase);
    std::string GenerateSeed();
    std::string GenerateSeed(std::string passphrase);
    std::string GetSeed();
    std::bitset<128> GetEntropy();
    std::string GetEntropyStr();
    std::string GetChecksum();
    std::string GetMnemonic();

};

#endif