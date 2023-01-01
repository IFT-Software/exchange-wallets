#ifndef BITCOIN_ADDRESS_H
#define BITCOIN_ADDRESS_H

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "bitcoin/pubkey.h"

class Script;

// notes: can have LEGACY for P2PKH and P2SH
enum class AddrType : uint16_t { P2PKH, P2SH };

class Address {
 private:
  AddrType addr_type_;
  std::string addr_;
  // std::vector<uint8_t> addr;

 public:
  Address(std::string& addr, AddrType& addr_type);

  bool IsValid(std::string& addr, AddrType& addr_type);
};

#endif