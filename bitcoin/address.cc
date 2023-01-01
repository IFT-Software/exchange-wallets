#include "bitcoin/address.h"

#include <cstdint>
#include <stdexcept>

#include "bitcoin/pubkey.h"
#include "bitcoin/scriptpubkey.h"
#include "util/crypto.h"

Address::Address(std::string& addr, AddrType& addr_type) {
  if (IsValid(addr, addr_type)) {
    addr_ = addr;
    addr_type_ = addr_type;
  } else {
    // todo: custom exceptions
    throw std::invalid_argument("invalid address");
  }
}

// todo: change this
bool Address::IsValid(std::string& addr, AddrType& addr_type) { return true; }
