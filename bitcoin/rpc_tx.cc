#include "bitcoin/rpc_tx.h"
#include <curl/urlapi.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "absl/strings/str_join.h"
#include "bitcoin/json_rpc.h"
#include "bitcoin/transaction.h"
#include "net/https.h"
#include "scriptpubkey.h"
#include "transaction.h"
#include "util/util.h"

RpcTx::RpcTx() : val_(nullptr), has_hex_(false) {}

RpcTx::RpcTx(json::value& val, bool has_hex) : val_(val), has_hex_(has_hex) {}

json::value RpcTx::GetData() { return val_; }

bool RpcTx::IsValid() { return (val_.is_null() ? false : true); }

std::string RpcTx::GetHex() {
  if (!IsValid()) {
    std::cerr << "Invalid RpcTx object" << std::endl;
    return "";
  }

  std::cout << val_ << std::endl;

  if (has_hex_) {
    return val_.as_object()["hex"].as_string().c_str();
  }

  std::string tx_id = val_.as_object()["txid"].as_string().c_str();

  RpcTx res;
  bitcoin::rpc::GetRawTransaction(tx_id, res);
  return res.GetHex();
}

uint32_t RpcTx::GetConfirmations() {
  if (!IsValid()) {
    std::cerr << "Invalid RpcTx object" << std::endl;
    return 0;
  }

  if (has_hex_) {
    return (uint32_t)val_.as_object()["confirmations"].as_int64();
  }

  std::string tx_id = val_.as_object()["tx_id"].as_string().c_str();

  RpcTx res;
  bitcoin::rpc::GetRawTransaction(tx_id, res);
  return res.GetConfirmations();
}

/**
 * @brief Throws an std::runtime_error exception if the json object cannot be parsed.
 *
 * @param input
 * @return Input*
 */
Input* ParseInput(json::object& input) {
  // TODO: is there any problem (memory-wise) to initialize the variables like this?
  std::string coinbase = "";
  std::string txid = "";
  uint32_t vout = 0;
  std::string script_sig = "";

  // these are declared here because of the try catch block.
  uint32_t seq;
  std::vector<std::string> witness;

  try {
    try {
      coinbase = input["coinbase"].as_string().c_str();
    } catch (std::exception& e) {
      txid = input["txid"].as_string().c_str();
      vout = (uint32_t)input["vout"].as_int64();
      script_sig = input["scriptSig"].as_object()["hex"].as_string().c_str();
    }

    json::array wit_arr;
    wit_arr = input["txinwitness"].as_array();
    for (json::value wit : wit_arr) {
      witness.push_back(wit.as_string().c_str());
    }

    seq = (uint32_t)input["sequence"].as_int64();

  } catch (std::exception& e) {
    std::cerr << "Error while parsing the input: " << e.what() << std::endl;
    throw std::runtime_error("Error while parsing the input");
  }

  std::array<uint8_t, 4> seq_bytes;
  util::UInt32ToBytes(seq, seq_bytes);

  Input* res = new Input(txid, vout, script_sig, witness, coinbase, seq_bytes);
  return res;
}

Output* ParseOutput(json::object& output) {
  double value_btc;
  uint64_t value;
  std::string address;
  std::string script_pubkey;
  std::string out_type_str;
  bool is_spendable;

  try {
    value_btc = output["value"].as_double();
    std::cout << "here 1" << std::endl;

    // TODO: not really sure what's going on here, can't cast double to int properly
    value = (uint64_t)(value_btc * pow(10.0, 9.0)) / 10;

    std::string script_asm = output["scriptPubKey"].as_object()["asm"].as_string().c_str();
    std::cout << "here 2" << std::endl;

    script_pubkey = output["scriptPubKey"].as_object()["hex"].as_string().c_str();
    std::cout << "here 3" << std::endl;

    out_type_str = output["scriptPubKey"].as_object()["type"].as_string().c_str();

    try {
      address = output["scriptPubKey"].as_object()["address"].as_string().c_str();
      is_spendable = true;
    } catch (std::exception& e) {
      address = "";
      is_spendable = false;
    }

  } catch (std::exception& e) {
    std::cerr << "Error while parsing the output: " << e.what() << std::endl;
    throw std::runtime_error("Error while parsing the output");
  }

  OutputType out_type;

  if (out_type_str == "pubkey") {
    out_type = OutputType::P2PK;
  } else if (out_type_str == "pubkeyhash") {
    out_type = OutputType::P2PKH;
  } else if (out_type_str == "scripthash") {
    out_type = OutputType::P2SH;
  } else if (out_type_str == "multisig") {
    out_type = OutputType::MULTISIG;
  } else if (out_type_str == "witness_v0_keyhash") {
    out_type = OutputType::WITNESS_V0_KEYHASH;
  } else if (out_type_str == "witness_v0_scripthash") {
    out_type = OutputType::WITNESS_V0_SCRIPTHASH;
  } else if (out_type_str == "witness_v1_taproot") {
    out_type = OutputType::UNKNOWN;
  } else if (out_type_str == "nulldata") {
    out_type = OutputType::NULL_DATA;
  } else {
    out_type = OutputType::UNKNOWN;
  }

  Output* res = (is_spendable) ? new Output(out_type, address, value, script_pubkey)
                               : new Output(out_type, value, script_pubkey);
  std::cout << "finished parsing output" << std::endl;
  return res;
}

Transaction* RpcTx::ParseTransaction() {
  std::string tx_id;
  uint32_t version;
  json::array input_arr;
  json::array output_arr;
  uint32_t locktime;

  std::cout << "val_ is: " << val_ << std::endl;

  try {
    tx_id = val_.as_object()["txid"].as_string().c_str();
    version = (uint32_t)(val_.as_object()["version"].as_int64());
    input_arr = val_.as_object()["vin"].as_array();
    output_arr = val_.as_object()["vout"].as_array();
    locktime = (uint32_t)val_.as_object()["locktime"].as_int64();

  } catch (std::exception& e) {
    std::cerr << "Error while parsing the transaction: " << e.what() << std::endl;
    throw std::runtime_error("Error while parsing the transaction");
  }

  uint32_t input_count = input_arr.size();
  std::vector<Input> inputs;
  for (uint32_t i = 0; i < input_count; i++) {
    Input* input = ParseInput(input_arr[i].as_object());
    inputs.push_back(*input);
  }

  uint32_t output_count = output_arr.size();
  std::vector<Output> outputs;
  for (uint32_t i = 0; i < output_count; i++) {
    Output* output = ParseOutput(output_arr[i].as_object());
    outputs.push_back(*output);
  }

  std::cout << "finished outputs" << std::endl;
  std::array<uint8_t, 4> locktime_bytes;
  util::UInt32ToBytes(locktime, locktime_bytes);

  std::cout << "finished outputs" << std::endl;

  Transaction* res = new Transaction(tx_id, version, input_count, inputs, output_count, outputs,
                                     locktime_bytes, true);

  std::cout << "finished parsing" << std::endl;
  return res;
};
