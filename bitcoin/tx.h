#ifndef BITCOIN_TX_H
#define BITCOIN_TX_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "util/util.h"

enum HashType {
  SIGHASH_ALL = 0x01,
  SIGHASH_NONE = 0x02,
  SIGHASH_SINGLE = 0x03,
  SIGHASH_ANYONECANPAY_ALL = 0x81,
  SIGHASH_ANYONECANPAY_NONE = 0x82,
  SIGHASH_ANYONECANPAY_SINGLE = 0x83
};

class Input {
 private:
  std::array<uint8_t, 32> tx_id_;
  std::array<uint8_t, 4> vout_;
  std::vector<uint8_t> sig_size_;
  std::vector<uint8_t> sig_;
  std::array<uint8_t, 4> sequence_;

 public:
  Input(std::array<uint8_t, 32> tx_id, std::array<uint8_t, 4> vout, std::vector<uint8_t> sig_size,
        std::vector<uint8_t> sig, std::array<uint8_t, 4> sequence);

  std::string hex();

  /**
   * @brief Returns the hex of this input with current signature removed, and replaced by
   * place_holder if is_replaced is true. If the signature is replaces, the signature size is
   * modified accordingly. Note that is_replaced is true when this is the input that will be signed
   * in a transaction.
   *
   * @param sig_size
   * @param place_holder
   * @param is_replaced
   * @return std::string
   */
  std::string ReturnUnsignedInput(std::vector<uint8_t>& sig_size,
                                  std::vector<uint8_t>& place_holder, bool is_replaced);
};

typedef struct Output_ {
  std::array<uint8_t, 8> value_;
  // TODO: need to be an array????
  std::vector<uint8_t> script_pubkey_size_;
  std::vector<uint8_t> script_pubkey_;

  std::string hex() {
    const std::string value_str = util::BytesToHex(value_);
    const std::string script_pubkey_size_str = util::BytesToHex(script_pubkey_size_);
    const std::string script_pubkey_str = util::BytesToHex(script_pubkey_);

    return value_str + script_pubkey_size_str + script_pubkey_str;
  }

} Output;

class Transaction {
 private:
  // TODO: look at this (constant? static?)
  std::string version_;
  std::vector<uint8_t> input_count_;
  std::vector<Input> inputs_;
  std::vector<uint8_t> output_count_;
  std::vector<Output> outputs_;
  // TODO: look at this
  std::array<uint8_t, 4> lock_time_;

 public:
  Transaction(std::string version, std::vector<uint8_t> input_count, std::vector<Input> inputs,
              std::vector<uint8_t> output_count, std::vector<Output> outputs,
              std::array<uint8_t, 4> lock_time);

  std::string hex();
  // TODO: (BILGE) other class functions

  /**
   * @brief Returns the hex of this transaction with existing signatures from the inputs other than
   * index are removed, and index's signature is replaced with the scriptPubKey of the transaction
   * it is referring to.
   *
   * @return Transaction
   */
  std::string ReturnUnsignedTransaction(uint32_t index);

  //   /**
  //    * @brief Appends signature hash type to transaction data. This is usually SIGHASH_ALL.
  //    *
  //    * @return std::string
  //    */
  //   std::string AppendHashType();
};

#endif