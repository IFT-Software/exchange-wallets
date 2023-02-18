#ifndef BITCOIN_TRANSACTION_H
#define BITCOIN_TRANSACTION_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "bitcoin/address.h"
#include "bitcoin/scriptpubkey.h"
#include "scriptpubkey.h"
#include "util/util.h"

enum HashType {
  SIGHASH_ALL = 0x01,
  SIGHASH_NONE = 0x02,
  SIGHASH_SINGLE = 0x03,
  SIGHASH_ANYONECANPAY_ALL = 0x81,
  SIGHASH_ANYONECANPAY_NONE = 0x82,
  SIGHASH_ANYONECANPAY_SINGLE = 0x83
};

/**
 * @brief The Input class can represent three types of inputs:
 * 1-Regular Non-Segwit Input
 * 2-Regular Segwit Input
 * 3-Coinbase Transaction Input
 *
 * For case 3, tx_id = "", vout = -1, sig = "", and sig_size = sizeof(coinbase). For case 1 and 2,
 * the witness and coinbase datas and the corresponding boolean values are set like explained
 * below.
 *
 */
class Input {
 private:
  std::string tx_id_;
  uint32_t vout_;
  // This is the size of the coinbase script if this is a coinbase input.
  std::vector<uint8_t> sig_size_;
  std::string sig_;
  std::array<uint8_t, 4> sequence_;

  // This is a zero-sized vector or a vector that contains empty/0x00 string if this inputs is not a
  // segwit input
  std::vector<std::string> witness_;

  // This is an empty string if this is an input of a coinbase transaction.
  std::string coinbase_;

  // Indicates that this input is coming from a segwit address (spending the outputs of a segwit
  // address). Set to true in the  constructor if the witness_ field is not empty.
  bool is_segwit_;

  // Indicates that this is the input of a coinbase transaction.
  bool is_coinbase_;

 public:
  /**
   * @brief Construct a new Input object. If the coinbase field is not the empty string, then
   * assumes the input is from a coinbase transaction, and set the other fields itself accordingly.
   *
   * @param tx_id
   * @param vout
   * @param sig
   * @param witness
   * @param sequence
   */
  Input(std::string tx_id, uint32_t vout, std::string sig, std::vector<std::string> witness,
        std::string coinbase, std::array<uint8_t, 4> sequence);

  std::string hex();

  bool IsSegwit() { return is_segwit_; }
  bool IsCoinbase() { return is_coinbase_; }

  /**
   * @brief If this input is a segwit input, returns its witness. If not, returns 0x00.
   *
   * @return std::string
   */
  std::string GetWitnessHex();

  /**
   * @brief Connects to the bitcoind and extracts the address from the @param index th output of the
   * transaction @param tx_id. This operation shouldn't be performed if this is a coinbase input.
   * @param tx_id
   * @param index
   * @return Address*
   */
  Address* ExtractAddress();

  // Getter functions. These functions will be primarily used to communicate the data to the
  // database in the correct format.
  // std::string Get
};

/**
 * @brief The Output class represents both spendable and unspendable outputs. The unspendable
 * outputs usually have OP_RETURN at the start of the scriptPubKey, which makes them unspendable.
 * The validity of the Output objects are not checked in the constructions. Use IsValid() method
 * instead.
 *
 */
class Output {
 private:
  OutputType out_type_;
  // This string is empty if the output is unspendable.
  std::string address_;

  uint64_t value_;
  std::vector<uint8_t> script_pubkey_size_;
  std::string script_pubkey_;

  // This boolean is set to false if the object is being constructed without an address, and true
  // otherwise.
  bool is_spendable_;

 public:
  // This constructs an unspendable outputs.
  Output(OutputType out_type, uint64_t value, std::string script_pubkey);
  // This constructs a spendable output.
  Output(OutputType out_type, std::string address, uint64_t value, std::string script_pubkey);

  std::string hex();

  bool IsSpendable() { return is_spendable_; }

  Address* ExtractAddress();
};

class Transaction {
 private:
  std::string tx_id_;
  uint32_t version_;
  uint32_t input_count_;
  std::vector<Input> inputs_;
  uint32_t output_count_;
  std::vector<Output> outputs_;
  std::array<uint8_t, 4> lock_time_;

  std::vector<Address> input_addresses_;
  std::vector<Address> output_addresses_;

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
   * @brief Constructer for a non-segwit and segwit transactions. Sets is_confirmed_ to false.
   * Invalid transactions are constructed, and their validity must be checked by calling IsValid().
   *
   * @param version
   * @param input_count
   * @param inputs
   * @param output_count
   * @param outputs
   * @param lock_time
   * @param is_segwit
   */
  Transaction(std::string& tx_id, uint32_t& version, uint32_t input_count,
              std::vector<Input>& inputs, uint32_t output_count, std::vector<Output>& outputs,
              std::array<uint8_t, 4>& lock_time, bool is_segwit);

  std::string hex();
  // TODO: other class functions

  /**
   * @brief Returns if this is a valid transaction.
   *
   * @return true
   * @return false
   */
  bool IsValid();

  /**
   * @brief Returns true if the transaction is a segwit transaction.
   *
   * @return true
   * @return false
   */
  bool IsSegwit();

  /**
   * @brief Makes this Transaction confirmed.
   *
   */
  void SetConfirmed();

  std::vector<Address> GetOutputAddresses();
  std::vector<Address> GetInputAddresses();

  // bool IsInputAddress(Address& addr);
  // bool IsOutputAddress(Address& addr);
};

#endif