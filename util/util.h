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
namespace {

[[maybe_unused]] std::string BytesToHex(const uint8_t* bytes, size_t len) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < len; i++) {
    ss << std::hex << std::setw(2) << (uint64_t)bytes[i];
  }
  return ss.str();
}

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

[[maybe_unused]] std::string HexToBin(const std::string& hexStr) {
  std::string binStr = "";
  for (size_t i = 0; i < hexStr.length(); i++) {
    switch (hexStr[i]) {
      case '0':
        binStr += "0000";
        break;
      case '1':
        binStr += "0001";
        break;
      case '2':
        binStr += "0010";
        break;
      case '3':
        binStr += "0011";
        break;
      case '4':
        binStr += "0100";
        break;
      case '5':
        binStr += "0101";
        break;
      case '6':
        binStr += "0110";
        break;
      case '7':
        binStr += "0111";
        break;
      case '8':
        binStr += "1000";
        break;
      case '9':
        binStr += "1001";
        break;
      case 'A':
      case 'a':
        binStr += "1010";
        break;
      case 'B':
      case 'b':
        binStr += "1011";
        break;
      case 'C':
      case 'c':
        binStr += "1100";
        break;
      case 'D':
      case 'd':
        binStr += "1101";
        break;
      case 'E':
      case 'e':
        binStr += "1110";
        break;
      case 'F':
      case 'f':
        binStr += "1111";
        break;
      default:
        break;
    }
  }
  return binStr;
}

[[maybe_unused]] std::vector<uint64_t> UnpackBitStr(const std::string& str, const size_t k,
                                                    const char filler) {
  std::string partition = "";
  std::vector<uint64_t> result;

  for (size_t i = 0; i < str.size(); i++) {
    partition += str[i];
    if (((i + 1) % k) == 0) {
      result.push_back(std::bitset<64>(partition).to_ullong());
      partition.clear();
    }
  }

  if (partition != "") {
    for (size_t i = partition.size(); i < k; i++) {
      partition.push_back(filler);
    }
    result.push_back(std::bitset<64>(partition).to_ullong());
  }
  return result;
}

[[maybe_unused]] std::vector<std::string> MakeWordList(const std::string& file_name) {
  std::vector<std::string> words;
  std::ifstream f;

  f.open(file_name, std::ifstream::in);  // open a file to perform read
                                         // operation using file object
  if (f.is_open()) {                     // checking whether the file is open
    std::string tp;
    while (getline(f, tp)) {  // read data from file object and put it into string.
      words.push_back(tp);    // print the data of the string
    }
    f.close();  // close the file object.
  }
  return words;
}

}  // namespace
}  // namespace util

#endif