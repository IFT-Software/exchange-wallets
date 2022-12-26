#include "bip32.h"

BIP32::BIP32()
{
  m_priv_key_ = new uint8_t[kPrivKeySize];
  m_pub_key_ = new uint8_t[kPubKeySize];
  chain_code_ = new uint8_t[kChainCodeSize];

  GeneratePrivKey();
}

BIP32::BIP32(uint8_t *seed)
{
  m_priv_key_ = new uint8_t[kPrivKeySize];
  m_pub_key_ = new uint8_t[kPubKeySize];
  chain_code_ = new uint8_t[kChainCodeSize];

  GeneratePrivKey(seed);
}

uint8_t *BIP32::GeneratePrivKey()
{
  BIP39 *seed_generator = new BIP39();
  std::string key = "Bitcoin seed";
  uint8_t *data = seed_generator->GetSeed();

  uint8_t result[64];
  Crypto::HMAC_SHA512(key, data, 64, result);

  std::copy(result, result + 32, m_priv_key_);
  std::copy(result + 32, result + 64, chain_code_);

  // Util::PickSubArray(result, this->m_priv_key, 0, 31);
  // Util::PickSubArray(result, this->m_pub_key, 32, 63);
  return m_priv_key_;
}

uint8_t *BIP32::GeneratePrivKey(uint8_t *seed)
{
  std::string key = "Bitcoin seed";
  uint8_t result[64];
  // std::cout << "hey3" << std::endl;

  Crypto::HMAC_SHA512(key, seed, 64, result);
  // std::cout << "hey2" << std::endl;
  std::cout << "HMAC Result: " << Util::BytesToHex(result, 64) << std::endl;

  std::copy(result, result + 32, m_priv_key_);
  std::copy(result + 32, result + 64, chain_code_);

  // Util::PickSubArray(result, this->m_priv_key, 0, 31);
  // Util::PickSubArray(result, this->m_pub_key, 32, 63);
  return m_priv_key_;
}

uint8_t *BIP32::GetPrivKey()
{
  return m_priv_key_;
}

std::string BIP32::GetPrivKeyStr()
{
  return Util::BytesToHex(m_priv_key_, 32);
}
