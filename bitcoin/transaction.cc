#include "bitcoin/transaction.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "absl/strings/str_join.h"
#include "net/https.h"
#include "pubkey.h"
#include "util/util.h"

Input::Input(std::string tx_id, uint32_t vout, std::string sig, std::vector<std::string> witness,
             std::array<uint8_t, 4> sequence)
    : tx_id_(tx_id), vout_(vout), sig_(sig), witness_(witness), sequence_(sequence) {
  util::UInt32ToBytes(sig.size() / 2, sig_size_);

  if (witness.size() != 0 && witness[0] != "") {
    is_segwit_ = true;
  } else {
    is_segwit_ = false;
  }
}

std::string Input::hex() {
  std::string tx_id_reversed = util::SwapEndian(tx_id_);

  std::array<uint8_t, 4> vout_bytes;
  util::UInt32ToBytes(vout_, vout_bytes);
  std::array<uint8_t, 4> vout_bytes_reversed;
  util::SwapEndian(vout_bytes, vout_bytes_reversed);
  const std::string vout_str = util::BytesToHex(vout_bytes_reversed);

  std::string sig_size_str = util::BytesToHex(sig_size_);
  const std::string sequence_str = util::BytesToHex(sequence_);

  return tx_id_reversed + vout_str + sig_size_str + sig_ + sequence_str;
}

std::string Input::json() {
  std::string json = "";

  json += "txid: " + tx_id_ + "\n";

  json += "vout: " + std::to_string(vout_);

  uint64_t script_size = util::BytesToUInt64(sig_size_);
  json += "script size: " + std::to_string(script_size) + "\n";

  json += "script sig: " + sig_ + "\n";

  std::string witness_str = "";
  for (std::string w : witness_) {
    witness_str += w + "\n";
  }
  json += "witness: " + witness_str;
  json += "sequence: " + util::BytesToHex(sequence_) + "\n";
  return json;
}

std::string Input::GetWitnessHex() {
  std::string witness = "";
  if (is_segwit_) {
    uint32_t n = witness_.size();
    std::vector<uint8_t> n_bytes;
    util::UInt32ToBytes(n, n_bytes);
    witness += util::BytesToHex(n_bytes);
    for (std::string w : witness_) {
      std::vector<uint8_t> size_bytes;
      util::UInt32ToBytes(w.size() / 2, size_bytes);
      witness += util::BytesToHex(size_bytes);
      witness += w;
    }
    return witness;
  } else {
    return "0x00";
  }
}

Address* Input::ExtractAddress() {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::string post_data = absl::StrCat(
      "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getrawtransaction\", "
      "\"params\": [\"",
      tx_id_, "\", true]}");

  std::string response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
                                          net::https::WriteType::TO_STRING, "", "anan", "anan");

  std::cout << response << std::endl;

  // TODO: throws boost::wrapexcept<std::bad_alloc> when buf size is 4096
  uint8_t buf[40096];
  json::static_resource mr(buf);
  json::parse_options opts;
  opts.allow_comments = true;
  opts.allow_trailing_commas = true;

  json::value val = json::parse(response, &mr, opts);

  json::array outputs;
  outputs = val.as_object()["result"].as_object()["vout"].as_array();

  std::string output_addr =
      outputs[vout_].as_object()["scriptPubKey"].as_object()["address"].as_string().c_str();

  std::string type =
      outputs[vout_].as_object()["scriptPubKey"].as_object()["type"].as_string().c_str();

  OutputType output_type;

  // TODO: not sure about the strings in here
  if (type == "pubkey") {
    output_type = OutputType::P2PK;
  } else if (type == "pubkeyhash") {
    output_type = OutputType::P2PKH;
  } else if (type == "script") {
    output_type = OutputType::P2SH;
  } else if (type == "witness_v0_keyhash") {
    output_type = OutputType::WITNESS_V0_KEYHASH;
  } else if (type == "witness_v0_scripthash") {
    output_type = OutputType::WITNESS_V0_SCRIPTHASH;
  } else if (type == "multisig") {
    output_type = OutputType::MULTISIG;
  } else {
    // TODO: for now
    output_type = OutputType::UNKNOWN;
  };

  Address* res = new Address(output_addr, output_type);
  return res;
}

Output::Output(OutputType out_type, std::string address, uint64_t value, std::string script_pubkey)
    : out_type_(out_type), address_(address), value_(value), script_pubkey_(script_pubkey) {
  util::UInt32ToBytes(script_pubkey.size() / 2, script_pubkey_size_);
};

std::string Output::hex() {
  std::array<uint8_t, 8> value_bytes;
  util::UInt64ToBytes(value_, value_bytes);
  std::array<uint8_t, 8> value_bytes_reversed;
  util::SwapEndian(value_bytes, value_bytes_reversed);

  const std::string value_str = util::BytesToHex(value_bytes_reversed);
  const std::string script_pubkey_size_str = util::BytesToHex(script_pubkey_size_);

  return value_str + script_pubkey_size_str + script_pubkey_;
}

std::string Output::json() {
  std::string json = "";

  std::array<uint8_t, 4> value_bytes;
  util::UInt32ToBytes(value_, value_bytes);
  json += "value: " + util::BytesToHex(value_bytes) + "\n";

  uint64_t script_pubkey_size = util::BytesToUInt64(script_pubkey_size_);
  json += "script pubkey size: " + std::to_string(script_pubkey_size) + "\n";

  json += "script pubkey: " + script_pubkey_ + "\n";
  return json;
}

Address* Output::ExtractAddress() {
  Address* res = new Address(address_, out_type_);
  return res;
}

Transaction::Transaction(std::string& tx_id, uint32_t& version, uint32_t input_count,
                         std::vector<Input>& inputs, uint32_t output_count,
                         std::vector<Output>& outputs, std::array<uint8_t, 4>& lock_time,
                         bool is_segwit)
    : tx_id_(tx_id),
      version_(version),
      input_count_(input_count),
      inputs_(inputs),
      output_count_(output_count),
      outputs_(outputs),
      lock_time_(lock_time),
      is_segwit_(is_segwit) {
  is_confirmed_ = false;

  for (Input i : inputs) {
    input_addresses_.push_back(*i.ExtractAddress());
  }

  for (Output o : outputs) {
    output_addresses_.push_back(*o.ExtractAddress());
  }
};

std::string Transaction::hex() {
  std::array<uint8_t, 4> version_bytes;
  util::UInt32ToBytes(version_, version_bytes);
  std::array<uint8_t, 4> version_bytes_reversed;
  util::SwapEndian(version_bytes, version_bytes_reversed);
  const std::string version_reversed = util::BytesToHex(version_bytes_reversed);

  std::vector<uint8_t> input_count_bytes;
  util::UInt32ToBytes(input_count_, input_count_bytes);
  const std::string input_count_str = util::BytesToHex(input_count_bytes);
  std::vector<uint8_t> output_count_bytes;
  util::UInt32ToBytes(output_count_, output_count_bytes);
  const std::string output_count_str = util::BytesToHex(output_count_bytes);

  bool is_segwit = false;

  std::string inputs = "";
  std::string witness = "";
  for (Input i : inputs_) {
    inputs = inputs + i.hex();
    witness += i.GetWitnessHex();
    is_segwit = i.IsSegwit() || is_segwit;
  };

  std::string outputs = "";
  for (Output o : outputs_) {
    outputs = outputs + o.hex();
  }

  std::array<uint8_t, 4> lock_time_reversed;
  util::SwapEndian(lock_time_, lock_time_reversed);
  const std::string lock_time_str = util::BytesToHex(lock_time_reversed);

  std::string marker_flag = "";
  if (is_segwit) {
    marker_flag = "0001";
  }

  return version_reversed + marker_flag + input_count_str + inputs + output_count_str + outputs +
         witness + lock_time_str;
}

std::string Transaction::json() {
  std::string json = "";
  json += "\nversion: " + std::to_string(version_) + "\n";

  std::string input_count = std::to_string(input_count_);
  json += "input count: " + input_count + "\n";

  for (int i = 0; i < input_count_; i++) {
    json += "input " + std::to_string(i) + ":\n";
    json += inputs_[i].json();
  }

  std::string output_count = std::to_string(output_count_);
  json += "output count: " + output_count + "\n";

  for (int i = 0; i < output_count_; i++) {
    json += "output " + std::to_string(i) + ":\n";
    json += outputs_[i].json();
  }

  json += "locktime: " + util::BytesToHex(lock_time_) + "\n";
  json += "is segwit: " + std::to_string(is_segwit_);
  return json;
}

std::vector<Address> Transaction::GetInputAddresses() { return input_addresses_; }
std::vector<Address> Transaction::GetOutputAddresses() { return output_addresses_; }
