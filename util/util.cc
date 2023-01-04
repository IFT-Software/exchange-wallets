#include "util/util.h"

#include <bitset>

namespace util {

[[maybe_unused]] std::string BytesToHex(const uint8_t* bytes, size_t len) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < len; i++) {
    ss << std::hex << std::setw(2) << (uint32_t)bytes[i];
  }
  return ss.str();
}

[[maybe_unused]] std::string BytesToHex(std::vector<uint8_t>& bytes) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < bytes.size(); i++) {
    ss << std::hex << std::setw(2) << (uint32_t)bytes[i];
  }
  return ss.str();
}

[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, uint8_t* bytes) {
  BinToBytes(std::bitset<16>(uint16), bytes);
}

[[maybe_unused]] void UInt16ToBytes(const uint16_t uint16, std::array<uint8_t, 2>& bytes) {
  UInt16ToBytes(uint16, bytes.begin());
}

[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, uint8_t* bytes) {
  BinToBytes(std::bitset<32>(uint32), bytes);
}

[[maybe_unused]] void UInt32ToBytes(const uint32_t uint32, std::array<uint8_t, 4>& bytes) {
  UInt32ToBytes(uint32, bytes.begin());
}

[[maybe_unused]] void UInt64ToBytes(const uint64_t uint64, uint8_t* bytes) {
  BinToBytes(std::bitset<64>(uint64), bytes);
}

[[maybe_unused]] void UInt64ToBytes(const uint64_t uint64, std::array<uint8_t, 8>& bytes) {
  UInt64ToBytes(uint64, bytes.begin());
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

[[maybe_unused]] std::string BytesToBin(const uint8_t* bytes, size_t len) {
  return HexToBin(BytesToHex(bytes, len));
}

[[maybe_unused]] std::string BytesToBin(std::vector<uint8_t>& bytes) {
  return HexToBin(BytesToHex(bytes));
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

}  // namespace util