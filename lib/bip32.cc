#include "bip32.h"

BIP32::BIP32()
{
  GeneratePrivKey();
}

BIP32::BIP32(std::array<uint8_t, 64> &seed)
{
  GeneratePrivKey(seed);
}

std::array<uint8_t, 32> BIP32::GetPrivKey()
{
  return m_priv_key_;
}

// uint8_t *BIP32::GetWIF()
// {
//   return wif_;
// }

std::string BIP32::GetPrivKeyStr()
{
  return Util::BytesToHex(m_priv_key_.begin(), 32);
}

// std::string BIP32::GetWIFStr()
// {
//   return Util::BytesToHex(wif_, 26);
// }

std::array<uint8_t, 4> BIP32::GenerateChecksum(std::array<uint8_t, 34> &extended)
{

  std::array<uint8_t, 32> res;
  std::array<uint8_t, 4> checksum;
  Crypto::SHA256(extended.begin(), 1 + 32 + 1, res.begin());
  Crypto::SHA256(res.begin(), res.size(), res.begin());

  std::copy(res.begin(), res.begin() + 4, checksum.begin());
  return checksum;
}

std::array<uint8_t, 32> BIP32::GeneratePrivKey()
{
  BIP39 *seed_generator = new BIP39();
  std::string key = "Bitcoin seed";
  std::array<uint8_t, 64> data = seed_generator->GetSeed();

  std::array<uint8_t, 64> result;
  Crypto::HMAC_SHA512(key, data.begin(), data.size(), result.begin());

  std::copy(result.begin(), result.begin() + 32, m_priv_key_.begin());
  std::copy(result.begin() + 32, result.end(), chain_code_.begin());

  // Util::PickSubArray(result, this->m_priv_key, 0, 31);
  // Util::PickSubArray(result, this->m_pub_key, 32, 63);
  return m_priv_key_;
}

std::array<uint8_t, 32> BIP32::GeneratePrivKey(std::array<uint8_t, 64> &seed)
{
  std::string key = "Bitcoin seed";
  uint8_t result[64];
  // std::cout << "hey3" << std::endl;

  Crypto::HMAC_SHA512(key, seed.begin(), 64, result);
  // std::cout << "hey2" << std::endl;
  std::cout << "HMAC Result: " << Util::BytesToHex(result, 64) << std::endl;

  std::copy(result, result + 32, m_priv_key_.begin());
  std::copy(result + 32, result + 64, chain_code_.begin());

  // Util::PickSubArray(result, this->m_priv_key, 0, 31);
  // Util::PickSubArray(result, this->m_pub_key, 32, 63);
  return m_priv_key_;
}

// uint8_t *BIP32::GenerateWIF(int net)
// {
//   uint8_t *prefix;
//   uint8_t suffix[] = {0x01};

//   if (net)
//   {
//     prefix[0] = {0x80};
//   }
//   else
//   {
//     prefix[0] = {0xEF};
//   };

//   uint8_t *extended = new uint8_t[34];
//   std::copy(prefix, prefix + 1, extended);
//   std::copy(m_priv_key_, m_ext_priv_key_ + 32, extended + 1);

//   uint8_t *checksum = GenerateChecksum(extended);

//   uint8_t *extended_checksum = new uint8_t[38];
// }
