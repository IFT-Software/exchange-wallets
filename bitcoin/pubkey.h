#ifndef BITCOIN_PUBKEY_H
#define BITCOIN_PUBKEY_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "bitcoin/crypto.h"

class Address;
enum class AddrType : uint16_t;
typedef std::array<uint8_t, 20> Hash160;

class PubKey {
 public:
  enum Size { kCompressed = 33, kUncompressed = 65 };
  enum LockType { p2pk = 00, p2pkh = 05 };

 private:
  std::array<uint8_t, Size::kUncompressed> pub_key_;

  uint32_t GetSize(uint8_t pubkey_first_byte) const;
  void Invalidate();

 public:
  PubKey();

  bool IsCompressed();
  bool IsUncompressed();
  bool IsValid();

  template <size_t N>
  PubKey(std::array<uint8_t, N>& pub_key) {
    if (!IsValid()) {
      throw std::invalid_argument("invalid public key");
    }
    if (N == Size::kCompressed || N == Size::kUncompressed) {
      std::copy(pub_key.begin(), pub_key.end(), pub_key_.begin());
    } else {
      Invalidate();
    }
  }

  uint32_t size() const;
  const uint8_t* data() const;
  const uint8_t* begin() const;
  const uint8_t* end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex() const;
  std::string bin() const;

  /**
   * @brief Converts this public key into uncompressed format (65 bytes).
   *
   * @return true
   * @return false
   */
  bool Decompress();

  /**
   * @brief Returns the hash160 of this public key.
   *
   * @return std::array<uint8_t, 20>
   */
  Hash160 GetHash160();
};

#endif