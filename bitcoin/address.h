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
  uint32_t user_id_;
  // std::vector<uint8_t> addr;

 public:
  Address(std::string& addr, OutputType& out_type);

  bool IsValid(std::string& addr, OutputType& out_type);

  /**
   * @brief Sets the used_id_ field of this Address object.
   *
   */
  void SetUserId(uint32_t id) { user_id_ = id; }

  // OutputType GetOutputType();
  std::string GetStr();

  /**
   * @brief Records this address to the database. Throws @exception ExistingAddressException if a
   * wallet with this seed exits.
   */
  void RecordDatabase() {}
};

#endif