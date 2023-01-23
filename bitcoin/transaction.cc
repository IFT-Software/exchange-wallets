#include "bitcoin/transaction.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "util/util.h"

Input::Input(std::array<uint8_t, 32> tx_id, std::array<uint8_t, 4> vout,
             std::vector<uint8_t> sig_size, std::vector<uint8_t> sig,
             std::array<uint8_t, 4> sequence)
    : tx_id_(tx_id), vout_(vout), sig_size_(sig_size), sig_(sig), sequence_(sequence) {}

std::string Input::hex() {
  const std::string tx_id_str = util::BytesToHex(tx_id_);
  const std::string vout_str = util::BytesToHex(vout_);
  const std::string sig_size_str = util::BytesToHex(sig_size_);
  const std::string sig_str = util::BytesToHex(sig_);
  const std::string sequence_str = util::BytesToHex(sequence_);

  return tx_id_str + vout_str + sig_size_str + sig_str + sequence_str;
}

std::string Input::json() {
  std::string json = "";

  json += "txid: " + util::BytesToHex(tx_id_) + "\n";

  // uint32_t vout = util::BytesToUInt32(vout_);
  // json += "vout: " + std::to_string(vout);
  json += "vout: " + util::BytesToHex(vout_);

  uint64_t script_size = util::BytesToUInt64(sig_size_);
  json += "script size: " + std::to_string(script_size) + "\n";

  json += "script sig: " + util::BytesToHex(sig_) + "\n";
  json += "sequence: " + util::BytesToHex(sequence_) + "\n";
  return json;
}

Output::Output(std::array<uint8_t, 8> value, std::vector<uint8_t> script_pubkey_size,
               std::vector<uint8_t> script_pubkey)
    : value_(value), script_pubkey_size_(script_pubkey_size), script_pubkey_(script_pubkey){};

std::string Output::hex() {
  const std::string value_str = util::BytesToHex(value_);
  const std::string script_pubkey_size_str = util::BytesToHex(script_pubkey_size_);
  const std::string script_pubkey_str = util::BytesToHex(script_pubkey_);

  return value_str + script_pubkey_size_str + script_pubkey_str;
}

std::string Output::json() {
  std::string json = "";

  uint64_t value = util::BytesToUInt64(value_);
  json += "value: " + std::to_string(value) + "\n";

  uint64_t script_pubkey_size = util::BytesToUInt64(script_pubkey_size_);
  json += "script pubkey size: " + std::to_string(script_pubkey_size) + "\n";

  json += "script pubkey: " + util::BytesToHex(script_pubkey_) + "\n";
  return json;
}

Transaction::Transaction(std::string version, std::vector<uint8_t> input_count,
                         std::vector<Input> inputs, std::vector<uint8_t> output_count,
                         std::vector<Output> outputs, std::array<uint8_t, 4> lock_time)
    : version_(version),
      input_count_(input_count),
      inputs_(inputs),
      output_count_(output_count),
      outputs_(outputs),
      lock_time_(lock_time) {
  is_segwit_ = false;
  is_confirmed_ = false;
};

Transaction::Transaction(std::string version, std::vector<uint8_t> input_count,
                         std::vector<Input> inputs, std::vector<uint8_t> output_count,
                         std::vector<Output> outputs, std::vector<uint8_t> witness,
                         std::array<uint8_t, 4> lock_time)
    : version_(version),
      input_count_(input_count),
      inputs_(inputs),
      output_count_(output_count),
      outputs_(outputs),
      witness_(witness),
      lock_time_(lock_time) {
  is_segwit_ = true;
  is_confirmed_ = false;
};

std::string Transaction::hex() {
  const std::string input_count_str = util::BytesToHex(input_count_);
  const std::string output_count_str = util::BytesToHex(output_count_);

  std::string inputs = "";
  for (Input i : inputs_) {
    inputs = inputs + i.hex();
  };

  std::string outputs = "";
  for (Output o : outputs_) {
    outputs = outputs + o.hex();
  }
  const std::string lock_time_str = util::BytesToHex(lock_time_);

  return version_ + input_count_str + inputs + output_count_str + outputs + lock_time_str;
}

std::string Transaction::json() {
  std::string json = "";
  json += "version: " + version_ + "\n";

  uint64_t input_count_int = util::BytesToUInt64(input_count_);
  std::string input_count = std::to_string(input_count_int);
  json += "input count: " + input_count + "\n";

  for (int i = 0; i < input_count_int; i++) {
    json += "input " + std::to_string(i) + "\n";
    json += inputs_[i].json();
  }

  uint64_t output_count_int = util::BytesToUInt64(output_count_);
  std::string output_count = std::to_string(output_count_int);
  json += "output count: " + output_count + "\n";

  for (int i = 0; i < output_count_int; i++) {
    json += "output " + std::to_string(i) + "\n";
    json += outputs_[i].json();
  }

  json += "locktime: " + util::BytesToHex(lock_time_);
  return json;
}
