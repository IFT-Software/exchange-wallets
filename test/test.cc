#include <array>

#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "gtest/gtest.h"
#include "test/consts.h"
#include "util/util.h"

using namespace test;

TEST(BytesToUIntConversions, BytesToUInt16Test) {
  uint8_t uint16_bytes1[] = {0x50, 0x61};
  std::array<uint8_t, 2> uint16_bytes_std1 = {0x50, 0x61};

  uint8_t uint16_bytes2[] = {0x00, 0x00};
  std::array<uint8_t, 2> uint16_bytes_std2 = {0x00, 0x00};

  EXPECT_EQ(util::BytesToUInt16(uint16_bytes1), (uint16_t)20577);
  EXPECT_EQ(util::BytesToUInt16(uint16_bytes_std1), (uint16_t)20577);

  EXPECT_EQ(util::BytesToUInt16(uint16_bytes2), (uint16_t)0);
  EXPECT_EQ(util::BytesToUInt16(uint16_bytes_std2), (uint16_t)0);
}

TEST(BytesToUIntConversions, BytesToUInt32Test) {
  uint8_t uint32_bytes1[] = {0x50, 0x61, 0x67, 0x65};
  std::array<uint8_t, 4> uint32_bytes_std1 = {0x50, 0x61, 0x67, 0x65};

  uint8_t uint32_bytes2[] = {0x00, 0x00, 0x00, 0x00};
  std::array<uint8_t, 4> uint32_bytes_std2 = {0x00, 0x00, 0x00, 0x00};

  EXPECT_EQ(util::BytesToUInt32(uint32_bytes1), (uint32_t)1348560741);
  EXPECT_EQ(util::BytesToUInt32(uint32_bytes_std1), (uint32_t)1348560741);

  EXPECT_EQ(util::BytesToUInt32(uint32_bytes2), (uint32_t)0);
  EXPECT_EQ(util::BytesToUInt32(uint32_bytes_std2), (uint32_t)0);
}

TEST(BytesToUIntConversions, BytesToUInt64Test) {
  uint8_t uint64_bytes1[] = {0x50, 0x61, 0x67, 0x65, 0x20, 0x6e, 0x6f, 0x74};
  std::array<uint8_t, 8> uint64_bytes_std1 = {0x50, 0x61, 0x67, 0x65, 0x20, 0x6e, 0x6f, 0x74};

  uint8_t uint64_bytes2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  std::array<uint8_t, 8> uint64_bytes_std2 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  EXPECT_EQ(util::BytesToUInt64(uint64_bytes1), (uint64_t)5792024279808634740LL);
  EXPECT_EQ(util::BytesToUInt64(uint64_bytes_std1), (uint64_t)5792024279808634740LL);

  EXPECT_EQ(util::BytesToUInt64(uint64_bytes2), (uint64_t)0);
  EXPECT_EQ(util::BytesToUInt64(uint64_bytes_std2), (uint64_t)0);
}

TEST(BIP39Test, SeedFromEntropy) {
  std::array<uint8_t, 64> seed;
  std::string seed_str;
  std::bitset<128> entropies[] = {entropy1, entropy2, entropy3, entropy4, entropy5};
  std::string exp_seeds[] = {seed1_str, seed2_str, seed3_str, seed4_str, seed5_str};

  for (int i = 0; i < 5; i++) {
    bitcoin::bip39::GenerateSeedFromEntropy(entropies[i], "", seed);
    seed_str = util::BytesToHex(seed.begin(), 64);
    EXPECT_STREQ(exp_seeds[i].c_str(), seed_str.c_str());
  }
}

class BitcoinCryptoTest : public ::testing::Test {
 protected:
  void SetUp() override {
    bitcoin::crypto::GeneratePrivKey(seed1, priv_keys[0]);
    bitcoin::crypto::GeneratePrivKey(seed2, priv_keys[1]);
    bitcoin::crypto::GeneratePrivKey(seed3, priv_keys[2]);
    bitcoin::crypto::GeneratePrivKey(seed4, priv_keys[3]);
    bitcoin::crypto::GeneratePrivKey(seed5, priv_keys[4]);
  }

 public:
  std::array<std::array<uint8_t, 32>, 5> priv_keys;
};

TEST_F(BitcoinCryptoTest, PrivKeyFromSeed) {
  std::string exp_privs[] = {prv1, prv2, prv3, prv4, prv5};

  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(util::BytesToHex(priv_keys[i]).c_str(), exp_privs[i].c_str());
  }
}

// TEST_F(BitcoinCryptoTest, WIFFromSeed) {
//   std::string wif;
//   std::string exp_wifs[] = {wif1, wif2, wif3, wif4, wif5};
//   for (int i = 0; i < 5; i++) {
//     wif = bitcoin::crypto::GenerateWIF(1, priv_keys[i].begin());
//   }

//   EXPECT_STREQ(wif.c_str(), wif1.c_str());
// }

// TEST(BitcoinCryptoTest, PubFromPriv) {
//   std::array<uint8_t, 32> priv_key;
//   bitcoin::crypto::GeneratePrivKey(seed1_b, priv_key);
//   std::array<uint8_t, 33> pub_key = bitcoin::crypto::GeneratePubKey(priv_key);
//   std::string pub_key_str = util::BytesToHex(pub_key);
//   EXPECT_STREQ(pub_key_str.c_str(), pub1.c_str());
// }

// TEST(UtilCryptoTest, RIPEMD160) {
//   std::array<uint8_t, 32> priv_key;
//   bitcoin::crypto::GeneratePrivKey(seed1, priv_key);
//   std::array<uint8_t, 33> pub_key = bitcoin::crypto::GeneratePubKey(priv_key);
//   std::array<uint8_t, 20> pub_key_hash;
//   bitcoin::crypto::GeneratePubKeyHash(pub_key, pub_key_hash);
//   std::string pub_key_hash_str = util::BytesToHex(pub_key_hash);
//   EXPECT_STREQ(pub_key_hash_str.c_str(), pub_hashed1.c_str());
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}