#include <secp256k1.h>

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "bitcoin/pubkey.h"
#include "util/util.h"

int main(int argc, char** argv) {
  // std::string seed =
  // "54ce95e2378ca7096e775f2c254d10f9a92b548e9378ac54297def659ae8d599dafc23dbae5f0e1130c3bd6e64cefffd6dc8cde38f2ba35811e480cd7246e675";
  std::array<uint8_t, 64> seed = {0x24, 0xd5, 0xe9, 0xfe, 0xbf, 0x08, 0xa9, 0x1d, 0xaa, 0x52, 0x43,
                                  0xa2, 0x5b, 0x2d, 0x4e, 0xc0, 0xf0, 0x95, 0x49, 0xb1, 0x95, 0x53,
                                  0x9c, 0x18, 0x8b, 0xca, 0xba, 0xcd, 0x7a, 0xee, 0xc6, 0x89, 0xc9,
                                  0x33, 0xc9, 0x42, 0xa5, 0x5e, 0x4e, 0x0e, 0xec, 0x65, 0x24, 0xe0,
                                  0x17, 0xc8, 0xb3, 0x1e, 0x5c, 0xd9, 0xea, 0x3f, 0x87, 0xb2, 0x9a,
                                  0xea, 0x12, 0x62, 0x02, 0xb2, 0xa2, 0x50, 0x55, 0xd3};
  // std::string wif1 = "L19aitXT5ryXvRHeRAgPAQaZ2ggjxD8Gs6hqDX8zodDRdAUwkaeN";
  std::string pub1_hex = "024db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978";
  std::string pub1_bin = util::HexToBin(pub1_hex);

  std::array<uint8_t, PubKey::Size::kCompressed> pub1_bytes;
  util::BinToBytes(std::bitset<PubKey::Size::kCompressed * 8>(pub1_bin), pub1_bytes);

  PubKey pk1 = PubKey(pub1_bytes);
  std::cout << "pub(cmp): " << pk1.hex() << std::endl;

  if (pk1.IsCompressed()) {
    std::cout << "This key is a compressed pub key" << std::endl;
  }

  if (pk1.IsUncompressed()) {
    std::cout << "This key is a uncompressed pub key" << std::endl;
  }

  std::string pub2_hex =
      "044db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978f585d9e0a2d30cc9137c1d64bc"
      "9887a5dde451571cb4f47fd12b8a334b436478";
  std::string pub2_bin = util::HexToBin(pub2_hex);

  std::array<uint8_t, PubKey::Size::kUncompressed> pub2_bytes;
  util::BinToBytes(std::bitset<PubKey::Size::kUncompressed * 8>(pub2_bin), pub2_bytes);

  PubKey pk2 = PubKey(pub2_bytes);
  std::cout << "pub(ucmp): " << pk2.hex() << std::endl;

  if (pk2.IsCompressed()) {
    std::cout << "This key is a compressed pub key" << std::endl;
  }

  if (pk2.IsUncompressed()) {
    std::cout << "This key is a uncompressed pub key" << std::endl;
  }

  // std::array<uint8_t, 32> p_key = bitcoin::crypto::GeneratePrivKey(seed);
  // std::cout << "prv: " << util::BytesToHex(p_key) << std::endl;

  return 0;
}
