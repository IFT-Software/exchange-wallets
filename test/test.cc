#include <array>
#include <stdexcept>

#include "gtest/gtest.h"

#include "bitcoin/address.h"
#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "bitcoin/privkey.h"
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
    for (int i = 0; i < 5; i++) {
      bitcoin::crypto::GeneratePrivKey(seeds[i], priv_keys[i]);
      bitcoin::crypto::GenerateExtPrivKey(seeds[i], ext_priv_keys[i]);
    };

    for (int i = 0; i < 5; i++) {
      bitcoin::crypto::GeneratePubKey(priv_keys[i], pub_keys[i]);
      bitcoin::crypto::GeneratePubKeyUncomp(priv_keys[i], unc_pub_keys[i]);
    };

    for (int i = 0; i < 5; i++) {
      bitcoin::crypto::GeneratePubKeyHash(pub_keys[i], pub_key_hashes[i]);
    };
  }

 public:
  std::array<std::array<uint8_t, 64>, 5> seeds = {seed1, seed2, seed3, seed4, seed5};
  std::array<std::array<uint8_t, 32>, 5> priv_keys;
  std::array<std::array<uint8_t, 64>, 5> ext_priv_keys;
  std::array<std::array<uint8_t, 33>, 5> pub_keys;
  std::array<std::array<uint8_t, 65>, 5> unc_pub_keys;
  std::array<std::array<uint8_t, 20>, 5> pub_key_hashes;
};

TEST_F(BitcoinCryptoTest, PrivKeyFromSeed) {
  std::string exp_privs[] = {prv1, prv2, prv3, prv4, prv5};

  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(util::BytesToHex(priv_keys[i]).c_str(), exp_privs[i].c_str());
  }
}

TEST_F(BitcoinCryptoTest, WIFFromSeed) {
  std::string exp_wifs[] = {wif1, wif2, wif3, wif4, wif5};
  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(bitcoin::crypto::GenerateWIF(1, priv_keys[i].begin()).c_str(),
                 exp_wifs[i].c_str());
  }
}

TEST_F(BitcoinCryptoTest, PubFromPriv) {
  std::string exp_pubs[] = {pub1, pub2, pub3, pub4, pub5};
  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(util::BytesToHex(pub_keys[i]).c_str(), exp_pubs[i].c_str());
  };
}

TEST_F(BitcoinCryptoTest, UncPubFromPriv) {
  std::string exp_unc_pubs[] = {unc_pub1, unc_pub2, unc_pub3, unc_pub4, unc_pub5};
  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(util::BytesToHex(unc_pub_keys[i]).c_str(), exp_unc_pubs[i].c_str());
  };
}

TEST_F(BitcoinCryptoTest, PubKeyHash) {
  std::string exp_hashes[] = {pub_hash1, pub_hash2, pub_hash3, pub_hash4, pub_hash5};
  for (int i = 0; i < 5; i++) {
    EXPECT_STREQ(util::BytesToHex(pub_key_hashes[i]).c_str(), exp_hashes[i].c_str());
  };

  // expect an exception
  std::array<uint8_t, 34> pub_key_long;
  std::copy(pub_keys[0].begin(), pub_keys[0].end(), pub_key_long.begin());
  pub_key_long[33] = 0x00;

  std::array<uint8_t, 20> res;
  EXPECT_ANY_THROW(bitcoin::crypto::GeneratePubKeyHash(pub_key_long.begin(), 34, res.begin()));
}

class BIP32Test : public ::testing::Test {
 protected:
  void SetUp() override {
    for (int i = 0; i < 5; i++) {
      bitcoin::crypto::GeneratePrivKey(seeds[i], priv_keys[i]);
      bitcoin::crypto::GenerateExtPrivKey(seeds[i], ext_priv_keys[i]);
    };
    for (int i = 0; i < 5; i++) {
      exts[i] = ExtPrivKey(ext_priv_keys[i]);
    };
  };

 public:
  std::array<std::array<uint8_t, 64>, 5> seeds = {seed1, seed2, seed3, seed4, seed5};
  std::array<std::array<uint8_t, 32>, 5> priv_keys;
  std::array<std::array<uint8_t, 64>, 5> ext_priv_keys;

  std::array<ExtPrivKey, 5> exts = {ext_priv_keys[0], ext_priv_keys[1], ext_priv_keys[2],
                                    ext_priv_keys[3], ext_priv_keys[4]};
};

TEST_F(BIP32Test, ChildDerivation) {
  PubKey pk;
  AddrType type = AddrType::P2PKH;
  std::string purpose_addr;

  std::string exp_derivations[] = {m_44__1, m_44__2, m_44__3, m_44__4, m_44__5};

  for (int i = 0; i < 5; i++) {
    pk = exts[i].DeriveNormalChild(44).GetPrivKey().GetPubKey();
    purpose_addr = bitcoin::crypto::GenerateAddressFromPubkey(pk, type);
    EXPECT_STREQ(exp_derivations[i].c_str(), purpose_addr.c_str());
  }

  pk = exts[0]
           .DeriveNormalChild(44)
           .DeriveNormalChild(0)
           .DeriveNormalChild(0)
           .DeriveNormalChild(0)
           .GetPrivKey()
           .GetPubKey();
  purpose_addr = bitcoin::crypto::GenerateAddressFromPubkey(pk, type);
  EXPECT_STREQ(m_44_0_0_0__1.c_str(), purpose_addr.c_str());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
