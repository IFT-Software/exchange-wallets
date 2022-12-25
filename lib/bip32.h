#ifndef LIB_BIP32_H
#define LIB_BIP32_H 

#include <string>

#include "bip39.h"
#include "crypto.h"

class BIP32 {

  private:
    uint8_t m_priv_key[32];
    uint8_t m_pub_key[32];
    uint8_t chain_code[32];

    uint8_t* GeneratePrivateKey();

  public:
    //generates master public and private keys from a random seed and 
    BIP32();
    //generates master public and private keys from a given seed
    BIP32(std::string seed);
};

#endif