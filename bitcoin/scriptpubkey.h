#ifndef BITCOIN_SCRIPTPUBKEY_H
#define BITCOIN_SCRIPTPUBKEY_H

#include <cstdint>
#include <string>
#include <vector>

enum LockType { p2pkh, p2sh };

class Script {
 private:
  LockType lock_type_;
  std::vector<uint8_t> script_;

 public:
  Script(std::vector<uint8_t> script, LockType lock_type);

  bool IsValid(std::vector<uint8_t> script, LockType lock_type);

  LockType GetLockType();

  uint32_t size() const;
  std::vector<uint8_t> data() const;
  std::vector<uint8_t>::const_iterator begin() const;
  std::vector<uint8_t>::const_iterator end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex();
  std::string bin();
};

#endif