#include "gtest/gtest.h"
#include "lib/bip39.h"
#include "lib/bip32.h"

TEST(HelloTest, BasicAssertions)
{
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(6 * 7, 42);
}

std::bitset<128> entropy1("00101101100101001101101110000001011000110011100100010100110111011111110010000100011101110010111111010100101000001001000001000000");
std::string seed1 = "24d5e9febf08a91daa5243a25b2d4ec0f09549b195539c188bcabacd7aeec689c933c942a55e4e0eec6524e017c8b31e5cd9ea3f87b29aea126202b2a25055d3";
uint8_t seed1_b[] = {0x24, 0xd5, 0xe9, 0xfe, 0xbf, 0x08, 0xa9, 0x1d,
                     0xaa, 0x52, 0x43, 0xa2, 0x5b, 0x2d, 0x4e, 0xc0,
                     0xf0, 0x95, 0x49, 0xb1, 0x95, 0x53, 0x9c, 0x18,
                     0x8b, 0xca, 0xba, 0xcd, 0x7a, 0xee, 0xc6, 0x89,
                     0xc9, 0x33, 0xc9, 0x42, 0xa5, 0x5e, 0x4e, 0x0e,
                     0xec, 0x65, 0x24, 0xe0, 0x17, 0xc8, 0xb3, 0x1e,
                     0x5c, 0xd9, 0xea, 0x3f, 0x87, 0xb2, 0x9a, 0xea,
                     0x12, 0x62, 0x02, 0xb2, 0xa2, 0x50, 0x55, 0xd3};
std::string prv1 = "7539c474d72a9cc9c89c67265bd8a5d4e37234b3a926e986ddff7cd02bc98510";

std::bitset<128> entropy2("01110100111101010001011101101000100110010101100110001010011100111001010111100001000110110000100110100110010100111011010010010111");
std::string seed2 = "43076aea18722554d3993bde416cf3cd43f08bcb04715f7e43b9130898af7d5b0312db488ebd6e8a14604da8c98eb8dbab792d3a1005d4c2b6d654d3598cd6c5";
std::string prv2 = "3bbb7fd52104cfc259811c588cf020b49bf8bfef3fe98de8fb518928112ee28e";

std::bitset<128> entropy3("10111001100011010100101101110001001010110110001010000101000000000010110011101111001001010110101101001011000010010111110010100111");
std::string seed3 = "07e5941b85ba9d1caf23e8f390f3ab29586abad259e274efb0205bbba84c490eae3a5f35c975824d63fe04343f0d94aefa1fd1bd98e8238e7a2ad085337ef3bf";
std::string prv3 = "a0986383f0d216d6643df2c8735500fb2d20c02ff734a4a0405be789a7c700bc";

std::bitset<128> entropy4("10101100011011110101001111100000111011101101000010100000100100001111101111010001010110000001100011010101111101101011111100010101");
std::string seed4 = "1214e21be823ed1dec76f9556b2eed65b2fac9dea6d9f217f9a058efcee5cf9bd49216047c35e6bfcaf3e6391a647f501e9be25917cb13e7be3b5f20fe038f8f";
std::string prv4 = "493348530495cd274af5a45f25361fd08a810fa19ba0fd576c6f6a5ebd2c743a";

std::bitset<128> entropy5("00101011011010000100101101000110001001110101011101101001111010111000110000100001011010101000101011110011001101100110010100100110");
std::string seed5 = "4351a04001eb6e5f8b4abf6edc948bd9f69a0b293654e15c40cff64b6d5ece83d32724a469914af1278867dc241f151e5624dfce2924b555c54dd5b395535193";
std::string prv5 = "0108c3c51c15dd5905246d105cc9b6a2ee393b2931dc1a04e0a7b4f40e1aaf47";

std::bitset<128> entropy6("00100101000111011110001101110110000100001000000000001010000111000101101001010110101100110101111110011101100111011101110000001010");
std::string seed6 = "ce3106608f816a30d7378653f7e16331d21ac94a58f35bc596adbd89199703b7a60651f3cec79b85eda46fa5e9d949bf9f7e003d4786b7253ebd802b5d2f9939";
std::string prv6 = "17bcca96cfb37cbcd570b54920badabe705bbf9c33d32fc8d082f0f07c9ec503";

std::bitset<128> entropy7("11000010110011110010010000101101100111100001100100010110101001001011000010110110110100000011110000110101100010000100101011101011");
std::string seed7 = "3cea2c21ef9bc0f56d55bc8f0a10b787a3a59701731ff4612c055a427b299c8614947e7caca28c6a14797d3b4c3d51803b837e1f2ce6923b6575708c4d29915b";
std::string prv7 = "a63cf2e5ca5e6ee7d177507cea28529b043ce92af11fd3462a07941ebe8364d5";

std::bitset<128> entropy8("10010000010000110111011110000100110011100111101001100111101110001101111001000110110011000101001100001010010000110000100000111011");
std::string seed8 = "5d43a052aa6cf67df370125c008081195e8892b006d1df51f33f976c55d699b79a4e0eb54a1a528258c179d21bd97ee785b4e18db9b2ffaf2c16a6285d89604f";
std::string prv8 = "7e0301c957c85276db827babff7d512fe0d03e387c3f154de8507980f1d24f68";

BIP39 seed_generator = BIP39();

TEST(BIP39Test, SeedFromEntropy)
{
  seed_generator.GenerateSeedFromEntropy(entropy1);
  EXPECT_STREQ(seed1.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy2);
  EXPECT_STREQ(seed2.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy3);
  EXPECT_STREQ(seed3.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy4);
  EXPECT_STREQ(seed4.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy5);
  EXPECT_STREQ(seed5.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy6);
  EXPECT_STREQ(seed6.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy7);
  EXPECT_STREQ(seed7.c_str(), seed_generator.GetSeedStr().c_str());

  seed_generator.GenerateSeedFromEntropy(entropy8);
  EXPECT_STREQ(seed8.c_str(), seed_generator.GetSeedStr().c_str());
}

BIP32 bip32 = BIP32();

TEST(BIP32Test, PrivKeyFromSeed)
{
  bip32.GeneratePrivKey(seed1_b);
  std::cout << "real value: " << bip32.GetPrivKeyStr() << std::endl;
  std::cout << "exp value: " << prv1;
  EXPECT_STREQ(bip32.GetPrivKeyStr().c_str(), prv1.c_str());

  // bip32.GeneratePrivKey(seed2_b);
  // EXPECT_EQ(bip32.GetPrivKeyStr(), prv2);

  // bip32.GeneratePrivKey(seed3_b);
  // EXPECT_EQ(bip32.GetPrivKeyStr(), prv3);

  // bip32.GeneratePrivKey(seed4_b);
  // EXPECT_EQ(bip32.GetPrivKeyStr(), prv4);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}