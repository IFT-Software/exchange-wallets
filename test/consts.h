#include <array>
#include <bitset>
#include <string>

#include "util/util.h"

namespace test {

std::array<uint8_t, 64> seedFromSeedStr(const std::string& seed_str) {
  std::array<uint8_t, 64> res;
  util::BinToBytes(std::bitset<64 * 8>(util::HexToBin(seed_str)), res);
  return res;
}

const std::bitset<128> entropy1(
    "0010110110010100110110111000000101100011001110010001010011011101111111001000010001110111001011"
    "1111010100101000001001000001000000");

const std::string seed1_str =
    "24d5e9febf08a91daa5243a25b2d4ec0f09549b195539c188bcabacd7aeec689c933c942a55e4e0eec6524e017c8b3"
    "1e5cd9ea3f87b29aea126202b2a25055d3";

std::array<uint8_t, 64> seed1 = seedFromSeedStr(seed1_str);

const std::string prv1 = "7539c474d72a9cc9c89c67265bd8a5d4e37234b3a926e986ddff7cd02bc98510";
const std::string wif1 = "L19aitXT5ryXvRHeRAgPAQaZ2ggjxD8Gs6hqDX8zodDRdAUwkaeN";

const std::string pub1 = "024db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978";
const std::string unc_pub1 =
    "044db2bc47838541eee14b8db5efde29c5201724021ecbf7ef6d9387e6b5ca2978f585d9e0a2d30cc9137c1d64bc98"
    "87a5dde451571cb4f47fd12b8a334b436478";
const std::string pub_hash1 = "f66a4fd55d468bd049579660b57504da32f8a924";
const std::string unc_pub_hash1 = "992714937a5e2235b98948bb7c7b1daab631295f";

const std::string address1 = "1PTvasHrVnkoK3Y8RL2zsLHNgTXSev2mDQ";

const std::string m_44__1 = "13BcVMa2CNF2GVvfnXoxsqsL9pHe5KgLdA";

const std::string m_44_0_0_0__1 = "1MPBpxT4SaXPk8NDewVNTgHramN4NYHFHA";

const std::bitset<128> entropy2(
    "0111010011110101000101110110100010011001010110011000101001110011100101011110000100011011000010"
    "0110100110010100111011010010010111");

const std::string seed2_str =
    "43076aea18722554d3993bde416cf3cd43f08bcb04715f7e43b9130898af7d5b0312db488ebd6e8a14604da8c98eb8"
    "dbab792d3a1005d4c2b6d654d3598cd6c5";

std::array<uint8_t, 64> seed2 = seedFromSeedStr(seed2_str);

const std::string prv2 = "3bbb7fd52104cfc259811c588cf020b49bf8bfef3fe98de8fb518928112ee28e";
const std::string wif2 = "KyDpgV7cHvpnvPesyq9jR5vRQ3iA6YVGcwF74jKiZUx1ed7r5ZhN";

const std::string pub2 = "037f06f6d79526d7009d9150a1a7727c17c47304228242e76e6c31ae533a3afaee";
const std::string unc_pub2 =
    "047f06f6d79526d7009d9150a1a7727c17c47304228242e76e6c31ae533a3afaee11c46f156c05c78ceb6e27a9b33d"
    "d91aeda2a29fe6aeb5bc5537e08e48da5e85";
const std::string pub_hash2 = "9a8f5885166e4ba42588bf548d856413b467a200";
const std::string unc_pub_hash2 = "ddaa51f54fcd93f6e72047cf0e7fc3dc7ef88819";

const std::string m_44__2 = "12w5zzawuMTJdfbJ7bKzzYoAKiBEqLZxAC";

const std::bitset<128> entropy3(
    "10111001100011010100101101110001001010110110001010000101000000000010110011101111001001"
    "01011010"
    "1101001011000010010111110010100111");

const std::string seed3_str =
    "07e5941b85ba9d1caf23e8f390f3ab29586abad259e274efb0205bbba84c490eae3a5f35c975824d63fe04343f0d94"
    "aefa1fd1bd98e8238e7a2ad085337ef3bf";

std::array<uint8_t, 64> seed3 = seedFromSeedStr(seed3_str);

const std::string prv3 = "a0986383f0d216d6643df2c8735500fb2d20c02ff734a4a0405be789a7c700bc";
const std::string wif3 = "L2btQT2k4fNJAe9FzzeYmT6dU32UmxZpUkaLj7QeeRsqUMBwhm5k";

const std::string pub3 = "02e901e3e5a4f5bbb8d6fa623e8daa71caea1463439b87f3c61fe419940f39a6b5";
const std::string unc_pub3 =
    "04e901e3e5a4f5bbb8d6fa623e8daa71caea1463439b87f3c61fe419940f39a6b5d49c1083908f1cd594f8125c3112"
    "9923aef3b7461ae3c282bc6750c1531227ca";
const std::string pub_hash3 = "7e8ac398f4eeb7b9634b9fd417b10cbdb8563981";
const std::string unc_pub_hash3 = "c57cf957b8e523abd10ee3e23ca2059722933f18";

const std::string m_44__3 = "1KUSZEbUDYK5RMkaBb9XBeHLUBFj7rxQ6q";

const std::bitset<128> entropy4(
    "1010110001101111010100111110000011101110110100001010000010010000111110111101000101011000000110"
    "0011010101111101101011111100010101");

const std::string seed4_str =
    "1214e21be823ed1dec76f9556b2eed65b2fac9dea6d9f217f9a058efcee5cf9bd49216047c35e6bfcaf3e6391a647f"
    "501e9be25917cb13e7be3b5f20fe038f8f";

std::array<uint8_t, 64> seed4 = seedFromSeedStr(seed4_str);

const std::string prv4 = "493348530495cd274af5a45f25361fd08a810fa19ba0fd576c6f6a5ebd2c743a";
const std::string wif4 = "Kyg17UkNTcHe6V9qatqUDeznSjcGv9Kv5PLb44dycyHTyX6yi5Yd";

const std::string pub4 = "02feef1c767d3f3db29a73ca4a02c5e1bbf021d5760b44104f8a7a741f70e0a019";
const std::string unc_pub4 =
    "04feef1c767d3f3db29a73ca4a02c5e1bbf021d5760b44104f8a7a741f70e0a0198ffbd808c1df35c669240244373e"
    "77b102656428b8addc0d9d802746e77e1c20";
const std::string pub_hash4 = "a6f70085368b2ece0981dbc845d7b5b24941ae22";
const std::string unc_pub_hash4 = "4153d32271c33b2e16d8c312c6adbf9ee025ecbc";

const std::string m_44__4 = "16W1YNgVudxkU67Ud4JkmjTyYYepPi1exz";

const std::bitset<128> entropy5(
    "0010101101101000010010110100011000100111010101110110100111101011100011000010000101101010100010"
    "1011110011001101100110010100100110");

std::string seed5_str =
    "4351a04001eb6e5f8b4abf6edc948bd9f69a0b293654e15c40cff64b6d5ece83d32724a469914af1278867dc241f15"
    "1e5624dfce2924b555c54dd5b395535193";

std::array<uint8_t, 64> seed5 = seedFromSeedStr(seed5_str);

const std::string prv5 = "0108c3c51c15dd5905246d105cc9b6a2ee393b2931dc1a04e0a7b4f40e1aaf47";
const std::string wif5 = "KwFiniaazpvkzZ59f9TyA4XvSKLcenA66Mv4Dk9VrygWfXVFBA1Y";

const std::string pub5 = "03500685859da82fc6af9470980523c9f115e529e4387e81267a5dd484c484f77f";
const std::string unc_pub5 =
    "04500685859da82fc6af9470980523c9f115e529e4387e81267a5dd484c484f77f6fa2dc446f4543c533ba866f2d81"
    "864f9a3484c085c98cb1b33d05381b68a1ed";
const std::string pub_hash5 = "ae224b7596c5942602810246d5a66784b3f88854";
const std::string unc_pub_hash5 = "6adde506e57844662fa39fde9b96438a27af93fb";

const std::string m_44__5 = "1FBJ9DAoZZ1HbaKMnA43x1TsUdHfbDW5tt";

std::bitset<128> entropy6(
    "0010010100011101111000110111011000010000100000000000101000011100010110100101011010110011010111"
    "1110011101100111011101110000001010");
std::string seed6 =
    "ce3106608f816a30d7378653f7e16331d21ac94a58f35bc596adbd89199703b7a60651f3cec79b85eda46fa5e9d949"
    "bf9f7e003d4786b7253ebd802b5d2f9939";
std::string prv6 = "17bcca96cfb37cbcd570b54920badabe705bbf9c33d32fc8d082f0f07c9ec503";

std::bitset<128> entropy7(
    "1100001011001111001001000010110110011110000110010001011010100100101100001011011011010000001111"
    "0000110101100010000100101011101011");
std::string seed7 =
    "3cea2c21ef9bc0f56d55bc8f0a10b787a3a59701731ff4612c055a427b299c8614947e7caca28c6a14797d3b4c3d51"
    "803b837e1f2ce6923b6575708c4d29915b";
std::string prv7 = "a63cf2e5ca5e6ee7d177507cea28529b043ce92af11fd3462a07941ebe8364d5";

std::bitset<128> entropy8(
    "1001000001000011011101111000010011001110011110100110011110111000110111100100011011001100010100"
    "1100001010010000110000100000111011");
std::string seed8 =
    "5d43a052aa6cf67df370125c008081195e8892b006d1df51f33f976c55d699b79a4e0eb54a1a528258c179d21bd97e"
    "e785b4e18db9b2ffaf2c16a6285d89604f";
std::string prv8 = "7e0301c957c85276db827babff7d512fe0d03e387c3f154de8507980f1d24f68";

}  // namespace test