#include "util/crypto.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "openssl/bio.h"
#include "openssl/evp.h"

namespace util {
namespace crypto {

[[maybe_unused]] void SHA256(const uint8_t* bytestream, size_t len, uint8_t* res) {
  uint8_t hash[EVP_MAX_MD_SIZE];
  uint32_t hash_len = 32;

  EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
  EVP_DigestUpdate(mdctx, bytestream, len);
  EVP_DigestFinal_ex(mdctx, hash, &hash_len);
  EVP_MD_CTX_free(mdctx);

  std::copy(hash, hash + hash_len, res);
}

[[maybe_unused]] void PBKDF2_HMAC_SHA512(const char* pass, const unsigned char* salt,
                                         int32_t salt_len, int32_t iterations, uint32_t keylength,
                                         uint8_t* res) {
  uint8_t digest[keylength];
  PKCS5_PBKDF2_HMAC(pass, std::strlen(pass), salt, salt_len, iterations, EVP_sha512(), keylength,
                    digest);

  std::copy(digest, digest + keylength, res);
}

[[maybe_unused]] void HMAC_SHA512(const uint8_t* key, size_t key_size, const uint8_t* data,
                                  size_t data_len, uint8_t* res) {
  uint8_t hash[EVP_MAX_MD_SIZE];
  size_t hash_len = 64;

  EVP_MD_CTX* evp_md = EVP_MD_CTX_new();
  EVP_PKEY* pkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, key, key_size);

  EVP_DigestSignInit(evp_md, NULL, EVP_sha512(), NULL, pkey);
  EVP_DigestSignUpdate(evp_md, data, data_len);
  EVP_DigestSignFinal(evp_md, hash, &hash_len);
  EVP_MD_CTX_free(evp_md);

  std::copy(hash, hash + hash_len, res);
}

[[maybe_unused]] void HMAC_SHA512(const std::string& key, const uint8_t* data, size_t data_len,
                                  uint8_t* res) {
  HMAC_SHA512((uint8_t*)key.c_str(), key.length(), data, data_len, res);
}

// todo: write base58 encoding with uint8_t array return type
[[maybe_unused]] std::string Base58Encode(const std::vector<uint8_t>& data, CodecMapping mapping) {
  std::vector<uint8_t> digits((data.size() * 138 / 100) + 1);
  size_t digitslen = 1;
  for (size_t i = 0; i < data.size(); i++) {
    uint32_t carry = static_cast<uint32_t>(data[i]);
    for (size_t j = 0; j < digitslen; j++) {
      carry = carry + static_cast<uint32_t>(digits[j] << 8);
      digits[j] = static_cast<uint8_t>(carry % 58);
      carry /= 58;
    }
    for (; carry; carry /= 58) digits[digitslen++] = static_cast<uint8_t>(carry % 58);
  }
  std::string result;
  for (size_t i = 0; i < (data.size() - 1) && !data[i]; i++)
    result.push_back(mapping.BaseMapping[0]);
  for (size_t i = 0; i < digitslen; i++)
    result.push_back(mapping.BaseMapping[digits[digitslen - 1 - i]]);
  return result;
}

[[maybe_unused]] void RIPEMD160(const uint8_t* data, size_t data_len, uint8_t* res) {
  uint32_t hash_len = 20;
  uint8_t hash[hash_len];

  EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, EVP_ripemd160(), NULL);
  EVP_DigestUpdate(mdctx, data, data_len);
  EVP_DigestFinal_ex(mdctx, hash, &hash_len);
  EVP_MD_CTX_free(mdctx);

  std::copy(hash, hash + hash_len, res);
}

}  // namespace crypto
}  // namespace util