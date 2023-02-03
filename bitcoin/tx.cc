#include "bitcoin/tx.h"
#include <curl/urlapi.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "absl/strings/str_join.h"
#include "bitcoin/transaction.h"
#include "net/https.h"
#include "scriptpubkey.h"
#include "util/util.h"

namespace bitcoin {
namespace tx {

Input* ParseInput(json::object& input) {
  std::string txid = input["txid"].as_string().c_str();
  // json::value::type_t type = input["vout"].type();

  // std::cout << "vout is integer: " << input["vout"].is_int64() << std::endl;
  uint32_t vout = (uint32_t)input["vout"].as_int64();

  std::string script_sig = input["scriptSig"].as_object()["hex"].as_string().c_str();
  std::vector<std::string> witness;

  json::array wit_arr;
  wit_arr = input["txinwitness"].as_array();
  for (json::value wit : wit_arr) {
    witness.push_back(wit.as_string().c_str());
  }

  uint32_t seq = (uint32_t)input["sequence"].as_int64();
  std::array<uint8_t, 4> seq_bytes;
  util::UInt32ToBytes(seq, seq_bytes);

  Input* res = new Input(txid, vout, script_sig, witness, seq_bytes);
  return res;
}

Output* ParseOutput(json::object& output) {
  double value_btc = output["value"].as_double();
  // std::cout << "value double: " << value_btc << std::endl;

  // TODO: not really sure what's going on here, can't cast double to int properly
  uint64_t value = (uint64_t)(value_btc * pow(10.0, 9.0)) / 10;
  std::string address = output["scriptPubKey"].as_object()["address"].as_string().c_str();

  std::string script_pubkey = output["scriptPubKey"].as_object()["hex"].as_string().c_str();
  std::string out_type_str = output["scriptPubKey"].as_object()["type"].as_string().c_str();
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
  } else {
    out_type = OutputType::UNKNOWN;
  }

  Output* res = new Output(out_type, address, value, script_pubkey);
  return res;
}

Transaction* ParseTransaction(json::value& data) {
  std::string tx_id = data.as_object()["txid"].as_string().c_str();
  uint32_t version = (uint32_t)(data.as_object()["version"].as_int64());

  json::array input_arr;
  input_arr = data.as_object()["vin"].as_array();

  uint32_t input_count = input_arr.size();

  std::vector<Input> inputs;
  for (uint32_t i = 0; i < input_count; i++) {
    Input* input = ParseInput(input_arr[i].as_object());
    inputs.push_back(*input);
  }

  json::array output_arr;
  output_arr = data.as_object()["vout"].as_array();

  uint32_t output_count = output_arr.size();

  std::vector<Output> outputs;
  for (uint32_t i = 0; i < output_count; i++) {
    Output* output = ParseOutput(output_arr[i].as_object());
    outputs.push_back(*output);
  }

  uint32_t locktime = (uint32_t)data.as_object()["locktime"].as_int64();
  std::array<uint8_t, 4> locktime_bytes;
  util::UInt32ToBytes(locktime, locktime_bytes);

  Transaction* res = new Transaction(tx_id, version, input_count, inputs, output_count, outputs,
                                     locktime_bytes, true);
  return res;
};

}  // namespace tx
}  // namespace bitcoin