#ifndef BITCOIN_PUBKEY_H
#define BITCOIN_PUBKEY_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

class PubKey {
 public:
  enum Size { kCompressed = 33, kUncompressed = 65 };

 private:
  std::array<uint8_t, Size::kUncompressed> pub_key_;

  uint32_t GetSize(uint8_t pubkey_first_byte) const;
  void Invalidate();

 public:
  template <size_t N>
  PubKey(std::array<uint8_t, N>& pub_key) {
    if (N == Size::kCompressed || N == Size::kUncompressed) {
      std::copy(pub_key.begin(), pub_key.end(), pub_key_.begin());
    } else {
      Invalidate();
    }
  }

  bool IsCompressed();
  bool IsUncompressed();

  uint32_t size() const;
  const uint8_t* data() const;
  const uint8_t* begin() const;
  const uint8_t* end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex() const;
  std::string bin() const;
};

#endif