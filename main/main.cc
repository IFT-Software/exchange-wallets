#include <string>
#include <iostream>

#include "lib/bip39.h"
#include "lib/bip32.h"
#include <secp256k1.h>


int main(int argc, char **argv)
{
  // std::string seed = "54ce95e2378ca7096e775f2c254d10f9a92b548e9378ac54297def659ae8d599dafc23dbae5f0e1130c3bd6e64cefffd6dc8cde38f2ba35811e480cd7246e675";
  std::array<uint8_t, 64> seed = {0x24, 0xd5, 0xe9, 0xfe, 0xbf, 0x08, 0xa9, 0x1d,
                                  0xaa, 0x52, 0x43, 0xa2, 0x5b, 0x2d, 0x4e, 0xc0,
                                  0xf0, 0x95, 0x49, 0xb1, 0x95, 0x53, 0x9c, 0x18,
                                  0x8b, 0xca, 0xba, 0xcd, 0x7a, 0xee, 0xc6, 0x89,
                                  0xc9, 0x33, 0xc9, 0x42, 0xa5, 0x5e, 0x4e, 0x0e,
                                  0xec, 0x65, 0x24, 0xe0, 0x17, 0xc8, 0xb3, 0x1e,
                                  0x5c, 0xd9, 0xea, 0x3f, 0x87, 0xb2, 0x9a, 0xea,
                                  0x12, 0x62, 0x02, 0xb2, 0xa2, 0x50, 0x55, 0xd3};
  std::string wif1 = "L19aitXT5ryXvRHeRAgPAQaZ2ggjxD8Gs6hqDX8zodDRdAUwkaeN";

  BIP39 *seedGenerator = new BIP39();
  // std::string seed = seedGenerator->GetSeed();
  // std::cout << "Seed: " << Util::BytesToHex(seed.begin(), 64) << std::endl;
  // std::cout << "Mnemonic: " << seedGenerator->GetMnemonic() << std::endl;
  // BIP32 *bip32 = new BIP32(seed);
  BIP32 *bip32 = new BIP32();
  // std::cout << "privkey: " << std::endl;
  // std::cout << "privkey: " << Util::BytesToHex(bip32->GeneratePrivKey(seed).begin(), 32) << std::endl;
  std::cout << "real: " << bip32->GenerateWIF(1, seed) << std::endl;
  // std::cout << "real: " << bip32->GenerateWIF(1, seed) << std::endl;
  std::cout << "expected: " << wif1 << std::endl;
  
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  return 0;
}
