#ifndef LIB_CRYPTO_H
#define LIB_CRYPTO_H

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>


namespace Crypto {
  
  static void sha256(const uint8_t bytestream[], size_t len, uint8_t* res) {
    uint8_t  hash[EVP_MAX_MD_SIZE]; 
    uint32_t hash_len = 32;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, bytestream, len);
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    std::copy(hash, hash + hash_len, res);
  }

  static void pbkdf2_hmac_sha_512(const char* pass, const unsigned char* salt, int32_t salt_len, int32_t iterations, uint32_t keylength, uint8_t* res) {
  
    uint8_t digest[keylength];
    PKCS5_PBKDF2_HMAC(pass, std::strlen(pass), salt, salt_len, iterations, EVP_sha512(), keylength, digest);

    std::copy(digest, digest + keylength, res);
  }


  static void hmac_sha512(std::string key, std::string data, uint8_t* res) {

    uint8_t hash[EVP_MAX_MD_SIZE];
    size_t hash_len = 64;

    EVP_MD_CTX* evp_md = EVP_MD_CTX_create();
    EVP_PKEY* pkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, (unsigned char*)&key[0], key.length());
    
    EVP_DigestSignInit(evp_md, NULL, EVP_sha512(), NULL, pkey);
    EVP_DigestSignUpdate(evp_md, (unsigned char*)&data[0], data.length());
    EVP_DigestSignFinal(evp_md, hash, &hash_len);

    std::copy(hash, hash + hash_len, res);
  }
}


#endif 