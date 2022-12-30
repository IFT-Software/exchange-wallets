#ifndef BITCOIN_PUBKEY_H
#define BITCOIN_PUBKEY_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

enum AddrType { P2PKH, P2SH };

class Address {
 private:
  AddrType addr_type;
  std::vector<uint8_t> addr;
};

class PubKey {
 public:
  enum Size { kCompressed = 33, kUncompressed = 65 };
  enum LockType { p2pk = 00, p2pkh = 05 };

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
  // todo
  bool IsValid();

  uint32_t size() const;
  const uint8_t* data() const;
  const uint8_t* begin() const;
  const uint8_t* end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex() const;
  std::string bin() const;

  // todo
  bool Decompress();

  /**
   * @brief Returns the hash160 of this public key.
   *
   * @return std::array<uint8_t, 20>
   */
  // todo: use kHASH160SIZE instead of 20
  std::array<uint8_t, 20> GetHash160();

  /**
   * @brief Get the address for this public key based on the address type
   *
   * @return Address
   */
  Address GetAddress(AddrType& addr_type);
};

#endif