#include "bip32.h"


BIP32::BIP32() {

  SeedGenerator* seed_generator = new SeedGenerator();
  std::string key = "Bitcoin seed";
  std::string data = seed_generator->GenerateSeed();

  uint8_t* result;
  Crypto::hmac_sha512(data, key, result);

  Util::PickSubArray(result, this->m_priv_key, 0, 31);
  Util::PickSubArray(result, this->m_pub_key, 32, 63);

}

BIP32::BIP32(std::string seed) {

  std::string key = "Bitcoin seed";
  uint8_t* result;
  Crypto::hmac_sha512(seed, key, result);

  Util::PickSubArray(result, this->m_priv_key, 0, 31);
  Util::PickSubArray(result, this->m_pub_key, 32, 63);

}



