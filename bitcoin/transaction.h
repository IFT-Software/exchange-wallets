#ifndef BITCOIN_TRANSACTION_H
#define BITCOIN_TRANSACTION_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

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

class Input {
 private:
  std::string tx_id_;
  uint32_t vout_;
  std::vector<uint8_t> sig_size_;
  std::string sig_;
  std::vector<std::string> witness_;
  std::array<uint8_t, 4> sequence_;

  /**
   * @brief Indicates that this input is coming from a segwit address. Set to true in the
   * constructor if the witness_ field is not empty.
   *
   */
  bool is_segwit_;

 public:
  Input(std::string tx_id, uint32_t vout, std::string sig, std::vector<std::string> witness,
        std::array<uint8_t, 4> sequence);

  std::string hex();
  std::string json();

  bool IsSegwit() { return is_segwit_; }

  /**
   * @brief If this input is a segwit input, returns its witness. If not, returns 0x00.
   *
   * @return std::string
   */
  std::string GetWitnessHex();
};

class Output {
 private:
  OutputType out_type_;

  uint64_t value_;
  std::vector<uint8_t> script_pubkey_size_;
  std::string script_pubkey_;

 public:
  Output(OutputType out_type, uint64_t value, std::string script_pubkey);

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
  std::string tx_id_;
  uint32_t version_;
  uint32_t input_count_;
  std::vector<Input> inputs_;
  uint32_t output_count_;
  std::vector<Output> outputs_;
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
   * @brief Constructer for a non-segwit transaction. Sets is_confirmed_ to false.
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
              std::vector<Input> inputs, uint32_t output_count, std::vector<Output> outputs,
              std::array<uint8_t, 4> lock_time, bool is_segwit);

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
};

#endif