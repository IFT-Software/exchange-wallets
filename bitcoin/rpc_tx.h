#ifndef BITCOIN_TX_H
#define BITCOIN_TX_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "bitcoin/address.h"
#include "bitcoin/json_rpc.h"
#include "bitcoin/scriptpubkey.h"
#include "bitcoin/transaction.h"
#include "util/util.h"

class RpcTx {
 private:
  json::value val_;
  bool has_hex_;

 public:
  RpcTx();
  RpcTx(json::value& val, bool has_hex);

  bool IsValid();

  /**
   * @brief Returns the txid of the transaction this object represents.
   *
   * @return std::string
   */
  std::string GetTxID();

  /**
   * @brief Returns the json value that this object represents.
   *
   * @return json::value
   */
  json::value GetData();

  /**
   * @brief Returns the hex-encoded string that represents this transaction. If this is an invalid
   * RpcTx object, then returns an empty string.
   *
   * @return std::string
   */
  std::string GetHex();

  /**
   * @brief Returns the number of confirmations that this transaction has. If this is an invalid
   * RpcTx object, then returns 0. This function makes a call to bitcoind when called because the
   * number of confirmations will change.
   *
   * @return uint32_t
   */
  uint32_t GetConfirmations();

  /**
   * @brief Parses the transaction data val_ which is a json value. Throws an exception if
   * the json value is not in the expected format. If the parsed json data is not a valid
   * representation of a bitcoin transaction, does not throw an exception.
   *
   * @return Transaction*
   * @throws std::runtime_error
   */
  Transaction* ParseTransaction();
};

#endif