#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <array>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "gmpxx.h"

namespace util {

[[maybe_unused]] std::string BytesToHex(const uint8_t* bytes, size_t len);

[[maybe_unused]] std::string BytesToHex(std::vector<uint8_t>& bytes);

template <std::size_t N>
[[maybe_unused]] std::string BytesToHex(std::array<uint8_t, N>& bytes) {
  return BytesToHex(bytes.begin(), bytes.size());
}

template <std::size_t N>
[[maybe_unused]] void BinToBytes(const std::bitset<N>& bin, uint8_t* bytes) {
  for (size_t i = 0; i < bin.size() / 8; i++) {
    uint8_t byte = 0;
    for (size_t j = 0; j < 8; j++) {
      byte <<= 1;
      byte ^= bin[(bin.size() - 1) - (i * 8 + j)];
    }

    bytes[i] = byte;
  }
}

template <std::size_t N, std::size_t M>
[[maybe_unused]] void BinToBytes(const std::bitset<N>& bin, std::array<uint8_t, M>& bytes) {
  return BinToBytes(bin, bytes.begin());
}

[[maybe_unused]] uint16_t BytesToUInt16(const uint8_t* bytes);
[[maybe_unused]] uint16_t BytesToUInt16(std::array<uint8_t, 2>& bytes);

[[maybe_unused]] uint32_t BytesToUInt32(const uint8_t* bytes);
[[maybe_unused]] uint32_t BytesToUInt32(std::array<uint8_t, 4>& bytes);

[[maybe_unused]] uint64_t BytesToUInt64(const uint8_t* bytes);
[[maybe_unused]] uint64_t BytesToUInt64(std::array<uint8_t, 8>& bytes);

[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, uint8_t* bytes);
[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, std::array<uint8_t, 2>& bytes);

[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, uint8_t* bytes);
[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, std::array<uint8_t, 4>& bytes);

[[maybe_unused]] void UInt64ToBytes(const uint64_t uint64, uint8_t* bytes);
[[maybe_unused]] void UInt64ToBytes(const uint64_t uint64, std::array<uint8_t, 8>& bytes);

[[maybe_unused]] void MpIntToBytes(const mpz_t& mpint, std::vector<uint8_t>& bytes);
[[maybe_unused]] void MpIntToBytes(const mpz_class& mpint, std::vector<uint8_t>& bytes);

[[maybe_unused]] std::string HexToBin(const std::string& hexStr);

[[maybe_unused]] std::string BytesToBin(const uint8_t* bytes, size_t len);

[[maybe_unused]] std::string BytesToBin(std::vector<uint8_t>& bytes);

[[maybe_unused]] std::vector<uint64_t> UnpackBitStr(const std::string& str, const size_t k,
                                                    const char filler);

[[maybe_unused]] std::vector<std::string> MakeWordList(const std::string& file_name);

}  // namespace util

#endif