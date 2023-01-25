#include "bitcoin/address.h"

#include <cstdint>
#include <stdexcept>

#include "bitcoin/pubkey.h"
#include "bitcoin/scriptpubkey.h"
#include "util/crypto.h"

Address::Address(std::string& addr, OutputType& out_type) {
  if (IsValid(addr, out_type)) {
    addr_ = addr;
    out_type_ = out_type;
  } else {
    // todo: custom exceptions
    throw std::invalid_argument("invalid address");
  }
}

// todo: change this
bool Address::IsValid(std::string& addr, OutputType& out_type) { return true; }
