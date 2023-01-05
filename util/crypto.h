#ifndef UTIL_CRYPTO_H
#define UTIL_CRYPTO_H

#include <openssl/bio.h>
#include <openssl/evp.h>

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace util {
namespace crypto {

[[maybe_unused]] constexpr const uint8_t kBase58Map[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

[[maybe_unused]] constexpr const uint8_t kAlphaMap[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0xff, 0x11, 0x12, 0x13, 0x14, 0x15, 0xff,
    0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0xff, 0x2c, 0x2d, 0x2e,
    0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff};

using CodecMapping = struct _codecmapping {
  _codecmapping(const uint8_t* amap, const uint8_t* bmap) : AlphaMapping(amap), BaseMapping(bmap) {}
  const uint8_t* AlphaMapping;
  const uint8_t* BaseMapping;
};

[[maybe_unused]] void SHA256(const uint8_t* bytestream, size_t len, uint8_t* res);

[[maybe_unused]] void PBKDF2_HMAC_SHA512(const char* pass, const unsigned char* salt,
                                         int32_t salt_len, int32_t iterations, uint32_t keylength,
                                         uint8_t* res);

[[maybe_unused]] void HMAC_SHA512(const uint8_t* key, size_t key_size, const uint8_t* data,
                                  size_t data_len, uint8_t* res);
[[maybe_unused]] void HMAC_SHA512(const std::string& key, const uint8_t* data, size_t data_len,
                                  uint8_t* res);

[[maybe_unused]] std::string Base58Encode(const std::vector<uint8_t>& data, CodecMapping mapping);

[[maybe_unused]] void RIPEMD160(const uint8_t* data, size_t data_len, uint8_t* res);

}  // namespace crypto
}  // namespace util

#endif