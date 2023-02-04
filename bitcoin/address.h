#ifndef BITCOIN_ADDRESS_H
#define BITCOIN_ADDRESS_H

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "bitcoin/pubkey.h"
#include "bitcoin/scriptpubkey.h"

class Script;

class Address {
 private:
  OutputType out_type_;
  std::string addr_;
  // std::vector<uint8_t> addr;

 public:
  Address(std::string& addr, OutputType& out_type);

  bool IsValid(std::string& addr, OutputType& out_type);

  // OutputType GetOutputType();
  std::string GetStr();
};

#endif