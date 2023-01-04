#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace util {

[[maybe_unused]] std::string BytesToHex(const uint8_t* bytes, size_t len);

[[maybe_unused]] std::string BytesToHex(std::vector<uint8_t>& bytes);

template <std::size_t N>
[[maybe_unused]] std::string BytesToHex(std::array<uint8_t, N>& bytes) {
  return BytesToHex(bytes.begin(), bytes.size());
}

template <std::size_t N>
[[maybe_unused]] void BinToBytes(const std::bitset<N>& bin, uint8_t* bytes) {
  std::bitset<128> mask(0);
  mask.flip();

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

[[maybe_unused]] std::string HexToBin(const std::string& hexStr);

[[maybe_unused]] std::string BytesToBin(const uint8_t* bytes, size_t len);

[[maybe_unused]] std::string BytesToBin(std::vector<uint8_t>& bytes);

[[maybe_unused]] std::vector<uint64_t> UnpackBitStr(const std::string& str, const size_t k,
                                                    const char filler);

[[maybe_unused]] std::vector<std::string> MakeWordList(const std::string& file_name);

}  // namespace util

#endif