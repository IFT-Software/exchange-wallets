#include <gmpxx.h>
#include <secp256k1.h>

#include <array>
#include <cstdint>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include "bitcoin/address.h"
#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "bitcoin/privkey.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/script.h"
#include "bitcoin/scriptpubkey.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/multiprecision/gmp.hpp"
#include "comms/comms.h"
#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "util/util.h"

int main(int argc, char** argv) {
  // zmq::context_t ctx(4);

  // // SubscriberThread(&ctx);
  // auto thread = std::async(std::launch::async, &(comms::SubscriberThread), &ctx);
  // thread.wait();

  // -----------------------------------------------------------------------------------------

  // std::string seed =

  //     "54ce95e2378ca7096e775f2c254d10f9a92b548e9378ac54297def659ae8d599dafc23dbae5f0e1130c3bd6e64ce"
  //     "fffd"
  //     "6dc8cde38f2ba35811e480cd7246e675";
  // std::array<uint8_t, 64> seed = {0x24, 0xd5, 0xe9, 0xfe, 0xbf, 0x08, 0xa9, 0x1d, 0xaa, 0x52,
  // 0x43,
  //                                 0xa2, 0x5b, 0x2d, 0x4e, 0xc0, 0xf0, 0x95, 0x49, 0xb1, 0x95,
  //                                 0x53, 0x9c, 0x18, 0x8b, 0xca, 0xba, 0xcd, 0x7a, 0xee, 0xc6,
  //                                 0x89, 0xc9, 0x33, 0xc9, 0x42, 0xa5, 0x5e, 0x4e, 0x0e, 0xec,
  //                                 0x65, 0x24, 0xe0, 0x17, 0xc8, 0xb3, 0x1e, 0x5c, 0xd9, 0xea,
  //                                 0x3f, 0x87, 0xb2, 0x9a, 0xea, 0x12, 0x62, 0x02, 0xb2, 0xa2,
  //                                 0x50, 0x55, 0xd3};
  // // std::string wif1 = "L19aitXT5ryXvRHeRAgPAQaZ2ggjxD8Gs6hqDX8zodDRdAUwkaeN";
  // std::string pub1_hex = "024db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978";
  // std::string address1 = "1PTvasHrVnkoK3Y8RL2zsLHNgTXSev2mDQ";
  // std::string taddress1 = "n3yssvNqJpC46A1k8u1NhFVhYT89bYUksP";
  // std::string pub1_bin = util::HexToBin(pub1_hex);

  // std::array<uint8_t, PubKey::Size::kCompressed> pub1_bytes;
  // util::BinToBytes(std::bitset<PubKey::Size::kCompressed * 8>(pub1_bin), pub1_bytes);

  // PubKey pk1 = PubKey(pub1_bytes);

  // std::cout << "pub(cmp): " << pk1.hex() << std::endl;

  // if (pk1.IsCompressed()) {
  //   std::cout << "This key is a compressed pub key" << std::endl;
  // }

  // if (pk1.IsUncompressed()) {
  //   std::cout << "This key is an uncompressed pub key" << std::endl;
  // }

  // AddrType type = AddrType::P2PKH;
  // std::string addr = bitcoin::crypto::GenerateAddressFromPubkey(pk1, type);
  // std::cout << "address 1: " << addr << std::endl;

  // Address addr_o = Address(addr, type);

  // std::array<uint8_t, 24> p2pkh_script1;
  // bitcoin::script::GenerateP2PKHScript(pk1, p2pkh_script1);

  // std::cout << util::BytesToHex(p2pkh_script1) << std::endl;

  // Script script_o =
  //     Script(std::vector<uint8_t>(p2pkh_script1.begin(), p2pkh_script1.end()), LockType::p2pkh);

  // //--------------------------------------------------------------------------------------------------------

  // std::array<uint8_t, 64> extended_priv_key;
  // bitcoin::crypto::GenerateExtPrivKey(seed, extended_priv_key);

  // std::cout << "\np2pkh script: " << util::BytesToHex(p2pkh_script1) << std::endl;

  // std::string pub2_hex =
  //     "044db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978f585d9e0a2d30cc9137c1d"
  //     "64bc"
  //     "9887a5dde451571cb4f47fd12b8a334b436478";
  // std::string pub2_bin = util::HexToBin(pub2_hex);

  // std::array<uint8_t, PubKey::Size::kUncompressed> pub2_bytes;
  // util::BinToBytes(std::bitset<PubKey::Size::kUncompressed * 8>(pub2_bin), pub2_bytes);

  // PubKey pk2 = PubKey(pub2_bytes);
  // std::cout << "pub(ucmp): " << pk2.hex() << std::endl;

  // if (pk2.IsCompressed()) {
  //   std::cout << "This key is a compressed pub key" << std::endl;
  // }

  // if (pk2.IsUncompressed()) {
  //   std::cout << "This key is a uncompressed pub key" << std::endl;
  // }

  // std::string priv1_hex = "7539c474d72a9cc9c89c67265bd8a5d4e37234b3a926e986ddff7cd02bc98510";
  // std::string priv1_bin = util::HexToBin(priv1_hex);

  // std::array<uint8_t, 32> priv1_bytes;
  // util::BinToBytes(std::bitset<32 * 8>(priv1_bin), priv1_bytes);

  // PrivKey pvk2 = PrivKey(priv1_bytes);
  // std::cout << "priv: " << pvk2.hex() << std::endl;

  // // std::array<uint8_t, 32> p_key = bitcoin::crypto::GeneratePrivKey(seed);
  // // std::cout << "prv: " << util::BytesToHex(p_key) << std::endl;

  // uint16_t uint16_int = 30;
  // uint32_t uint32_int = 30;
  // uint64_t uint64_int = 30;

  // uint8_t uint16_bytes[2];
  // uint8_t uint32_bytes[4];
  // uint8_t uint64_bytes[8];

  // util::UInt16ToBytes(uint16_int, uint16_bytes);
  // util::UInt32ToBytes(uint32_int, uint32_bytes);
  // util::UInt64ToBytes(uint64_int, uint64_bytes);

  // std::cout << "uint16_str: " << util::BytesToHex(uint16_bytes, 2) << std::endl;
  // std::cout << "uint32_str: " << util::BytesToHex(uint32_bytes, 4) << std::endl;
  // std::cout << "uint64_str: " << util::BytesToHex(uint64_bytes, 8) << std::endl;

  // namespace mpl = boost::multiprecision;

  // // int128_t v = 1;

  // // // Do some fixed precision arithmetic:
  // // for (unsigned i = 1; i <= 20; ++i) v *= i;

  // // std::cout << v << std::endl;  // prints 2432902008176640000 (i.e. 20!)

  // // Repeat at arbitrary precision:
  // mpz_int u = 1;
  // for (unsigned i = 1; i <= 10000; ++i) u *= i;

  // // prints
  // //
  // 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
  // // (i.e. 100!)
  // std::cout << u << std::endl;

  mpz_class mc("50616765206e6f74206e6f74", 16);
  mc = mc / 2;

  std::cout << mc << std::endl;

  std::array<uint8_t, 2> uint16_bytes = {0x50, 0x61};
  std::cout << util::BytesToUInt16(uint16_bytes) << std::endl;

  std::array<uint8_t, 4> uint32_bytes = {0x50, 0x61, 0x67, 0x65};
  std::cout << util::BytesToUInt32(uint32_bytes) << std::endl;

  std::array<uint8_t, 8> uint64_bytes = {0x50, 0x61, 0x67, 0x65, 0x20, 0x6e, 0x6f, 0x74};
  std::cout << util::BytesToUInt64(uint64_bytes) << std::endl;

  return 0;
}
