#ifndef BITCOIN_TRANSACTION_H
#define BITCOIN_TRANSACTION_H

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
  std::string json();

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

class Output {
 private:
  std::array<uint8_t, 8> value_;
  // TODO: need to be an array????
  std::vector<uint8_t> script_pubkey_size_;
  std::vector<uint8_t> script_pubkey_;

 public:
  Output(std::array<uint8_t, 8> value, std::vector<uint8_t> script_pubkey_size,
         std::vector<uint8_t> script_pubkey);

  std::string hex();
  std::string json();
};

class RawTransaction {
  // class invariant: none of the inputs are signed.
 private:
  std::string version;
  std::vector<uint8_t> input_count_;
  std::vector<Input> inputs_;
  std::vector<uint8_t> output_count_;
  std::vector<Output> outputs_;
  std::array<uint8_t, 4> lock_time_;

  /**
   * @brief serializes this transaction in order to sign one of the legacy inputs.
   *
   * @return std::string
   */
  std::string SerializeLegacy();

  /**
   * @brief serializes this transaction in order to sign one of the segwit inputs. see
   * bip 143 for more detailed info about the format of the serialization.
   *
   * @return std::string
   */
  std::string SerializeSegwit();
};

class Transaction {
 private:
  // TODO: look at this (constant? static?)
  std::string version_;
  std::vector<uint8_t> input_count_;
  std::vector<Input> inputs_;
  std::vector<uint8_t> output_count_;
  std::vector<Output> outputs_;
  // this may be empty if this is a non-segwit transaction
  // TODO: make this a type
  std::vector<uint8_t> witness_;
  // TODO: look at this
  std::array<uint8_t, 4> lock_time_;

  /**
   * @brief indicates if this is a segwit transaction. if this is true, that means
   * transaction data has a flag, marker, and witness. note that if a transaction has
   * at least one segwit input (native or wrapped), then it is a segwit transaction.
   *
   */
  bool is_segwit_;

  /**
   * @brief indicates if this transaction is still in the mempool (unconfirmed), or included
   * in a block (confirmed). False when a Transaction is first created.
   *
   */
  bool is_confirmed_;

 public:
  /**
   * @brief Constructer for a non-segwit transaction.
   *
   * @param version
   * @param input_count
   * @param inputs
   * @param output_count
   * @param outputs
   * @param lock_time
   */
  Transaction(std::string version, std::vector<uint8_t> input_count, std::vector<Input> inputs,
              std::vector<uint8_t> output_count, std::vector<Output> outputs,
              std::array<uint8_t, 4> lock_time);

  /**
   * @brief Constructer for a segwit transaction.
   *
   * @param version
   * @param input_count
   * @param inputs
   * @param output_count
   * @param outputs
   * @param witness
   * @param lock_time
   */
  Transaction(std::string version, std::vector<uint8_t> input_count, std::vector<Input> inputs,
              std::vector<uint8_t> output_count, std::vector<Output> outputs,
              std::vector<uint8_t> witness, std::array<uint8_t, 4> lock_time);

  std::string hex();
  std::string json();
  // TODO: other class functions

  /**
   * @brief Returns if this is a valid transaction.
   *
   * @return true
   * @return false
   */
  bool IsValid();

  /**
   * @brief Returns true if the transaction has a witness field (wtxid).
   *
   * @return true
   * @return false
   */
  bool HasWitness();
};

#endif