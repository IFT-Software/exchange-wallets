#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <array>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "boost/json.hpp"
#include "third_party/gmpxx/gmpxx.h"

namespace json = boost::json;

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
[[maybe_unused]] uint64_t BytesToUInt64(std::vector<uint8_t>& bytes);

[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, uint8_t* bytes);
[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, std::array<uint8_t, 2>& bytes);

[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, uint8_t* bytes);
[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, std::array<uint8_t, 4>& bytes);
[[maybe_unused]] void UInt32ToBytes(uint32_t uint32, std::vector<uint8_t>& bytes);

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

[[maybe_unused]] std::string JsonValueToSQLFormattedStr(json::value value);

/**
 * @brief Swaps the endiannes of the bytes of the @param input and writes the resuting vector
 * to @param res.
 *
 * @param input
 * @param res
 */
[[maybe_unused]] void SwapEndian(const std::vector<uint8_t>& input, std::vector<uint8_t>& res);

/**
 * @brief Swaps the endiannes of the bytes of the @param input and writes the resuting array
 * to @param res.
 *
 * @param input
 * @param res
 */
template <std::size_t N>
[[maybe_unused]] void SwapEndian(const std::array<uint8_t, N>& input, std::array<uint8_t, N>& res) {
  std::copy(input.begin(), input.end(), res.begin());
  for (size_t i = 0; i < res.size() / 2; ++i) {
    std::swap(res[i], res[res.size() - i - 1]);
  }
}

/**
 * @brief Reads the string input as a hex value and swaps the endiannes accordingly. Returns the
 * swapped string.
 * Invariant: The size of the input is even.
 *
 * @param input
 * @return std::string
 */
[[maybe_unused]] std::string SwapEndian(const std::string& input);

}  // namespace util

#endif