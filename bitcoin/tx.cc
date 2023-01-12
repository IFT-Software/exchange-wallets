#include "bitcoin/tx.h"

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

// std::string Input::ReturnUnsigned() {
//   const std::string tx_id_str = util::BytesToHex(tx_id_);
//   const std::string vout_str = util::BytesToHex(vout_);
//   const std::string sig_size_str = util::BytesToHex(0x00, 1);
//   const std::string sequence_str = util::BytesToHex(sequence_);
//   return tx_id_str + vout_str + sig_size_str + sequence_str;
// }

// std::string Input::ReturnReplacedSignature(std::vector<uint8_t>& sig_size,
//                                            std::vector<uint8_t>& place_holder) {
//   const std::string tx_id_str = util::BytesToHex(tx_id_);
//   const std::string vout_str = util::BytesToHex(vout_);
//   const std::string sig_size_str = util::BytesToHex(sig_size);
//   const std::string sig_str = util::BytesToHex(place_holder);
//   const std::string sequence_str = util::BytesToHex(sequence_);

//   return tx_id_str + vout_str + sig_size_str + sig_str + sequence_str;
// }

std::string Input::ReturnUnsignedInput(std::vector<uint8_t>& sig_size,
                                       std::vector<uint8_t>& place_holder, bool is_replaced) {
  const std::string tx_id_str = util::BytesToHex(tx_id_);
  const std::string vout_str = util::BytesToHex(vout_);
  const std::string sig_size_str = util::BytesToHex(sig_size);
  const std::string sig_str = util::BytesToHex(place_holder);
  const std::string sequence_str = util::BytesToHex(sequence_);

  if (is_replaced) {
    return tx_id_str + vout_str + sig_size_str + sig_str + sequence_str;
  } else {
    return tx_id_str + vout_str + util::BytesToHex(0x00, 1) + sequence_str;
  }
}

Output::Output(std::array<uint8_t, 8> value, std::vector<uint8_t> script_pubkey_size,
               std::vector<uint8_t> script_pubkey)
    : value_(value), script_pubkey_size_(script_pubkey_size), script_pubkey_(script_pubkey){};

Transaction::Transaction(std::string version, std::vector<uint8_t> input_count,
                         std::vector<Input> inputs, std::vector<uint8_t> output_count,
                         std::vector<Output> outputs, std::array<uint8_t, 4> lock_time)
    : version_(version),
      input_count_(input_count),
      inputs_(inputs),
      output_count_(output_count),
      outputs_(outputs),
      lock_time_(lock_time){};

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

// std::string Transaction::ReturnUnsignedTransaction(uint32_t index) {
//   std::string inputs_str = "";
//   for (int i = 0; i < inputs_.size(); i++) {
//     if (index = i) {
//       inputs_str += inputs_[i].ReturnUnsignedInput()
//     }
//   }
// }

namespace bitcoin {
namespace tx {

/**
 * @brief Looks at the byte at data[iterator], if it is
 * 0xfd: writes the following 2 bytes to the result vector.
 * 0xfe: writes the following 4 bytes to the result vector.
 * 0xff: writes the following 8 bytes to the result vector.
 * if none of the above, writes the following byte to the result vector.
 * increases the iterator value accordingly.
 *
 */
uint32_t VarInt(uint8_t* data, uint32_t iterator, std::vector<uint8_t>& res) {
  switch (*(data + iterator)) {
    case 0xfd:
      res.insert(res.begin(), data + iterator, data + iterator + 2);
      // iterator += 2;
      return 2;
    case 0xfe:
      res.insert(res.begin(), data + iterator, data + iterator + 4);
      // iterator += 4;
      return 4;
    case 0xff:
      res.insert(res.begin(), data + iterator, data + iterator + 8);
      // iterator += 8;
      return 8;
    default:
      res.insert(res.begin(), data + iterator, data + iterator + 1);
      // iterator += 1;
      return 1;
  }
}

Input* ParseInput(uint8_t* data, uint32_t& iterator) {
  // std::cout << "iterator enter parse input: " << iterator << std::endl;
  // std::cout << "parse input 1" << std::endl;
  std::array<uint8_t, 32> prev_tx;
  std::copy(data + iterator, data + iterator + 32, prev_tx.begin());
  iterator += 32;
  // std::cout << "parse input 2" << std::endl;

  std::array<uint8_t, 4> vout;
  std::copy(data + iterator, data + iterator + 4, vout.begin());
  iterator += 4;

  // std::cout << "parse input 3" << std::endl;

  std::vector<uint8_t> sig_size;
  iterator += VarInt(data, iterator, sig_size);
  uint64_t sig_size_int = util::BytesToUInt64(sig_size);
  // std::cout << "parse input 4" << std::endl;

  std::vector<uint8_t> sig(sig_size_int);
  std::cout << "signature size: " << sig_size_int << std::endl;
  // std::cout << "sig copy begin: " << iterator << std::endl;
  // std::cout << "sig copy end: " << iterator + sig_size_int << std::endl;
  std::copy(data + iterator, data + iterator + sig_size_int, sig.begin());
  iterator += sig_size_int;
  // std::cout << "parse input 5" << std::endl;

  std::array<uint8_t, 4> seqeunce;
  std::copy(data + iterator, data + iterator + 4, seqeunce.begin());
  iterator += 4;
  // std::cout << "parse input 6" << std::endl;

  Input* res = new Input(prev_tx, vout, sig_size, sig, seqeunce);
  return res;
}

Output* ParseOutput(uint8_t* data, uint32_t& iterator) {
  std::cout << "iterator when parsing the output " << iterator << std::endl;
  std::array<uint8_t, 8> value;
  std::copy(data + iterator, data + iterator + 8, value.begin());
  iterator += 8;

  std::cout << "output value: " << util::BytesToHex(value) << std::endl;

  std::vector<uint8_t> script_pubkey_size;
  iterator += VarInt(data, iterator, script_pubkey_size);
  uint64_t script_pubkey_size_int = util::BytesToUInt64(script_pubkey_size);

  std::cout << "script pubkey size " << util::BytesToHex(script_pubkey_size) << std::endl;

  std::vector<uint8_t> script_pubkey(script_pubkey_size_int);
  std::copy(data + iterator, data + iterator + script_pubkey_size_int, script_pubkey.begin());
  iterator += script_pubkey_size_int;
  // std::cout << "parse out end1" << std::endl;

  std::cout << "script pubkey " << util::BytesToHex(script_pubkey) << std::endl;

  Output* res = new Output(value, script_pubkey_size, script_pubkey);

  // std::cout << "parse out end2" << std::endl;
  return res;
}

Transaction* ParseTransaction(uint8_t* data) {
  // std::cout << "here3" << std::endl;

  uint32_t iterator = 0;

  std::array<uint8_t, 4> version;
  std::copy(data, data + 4, version.begin());
  std::string version_str = util::BytesToHex(version);

  // std::cout << "here4" << std::endl;

  iterator += 4;

  std::array<uint8_t, 2> flag;
  std::copy(data + iterator, data + iterator + 2, flag.begin());

  uint32_t flag_int = util::BytesToUInt16(flag);

  // std::cout << "here5" << std::endl;

  if (flag_int == 1) {
    iterator += 2;
  }

  std::vector<uint8_t> input_count;
  iterator += VarInt(data, iterator, input_count);

  // std::cout << "here6" << std::endl;

  std::vector<Input> inputs;
  uint64_t input_count_int = util::BytesToUInt64(input_count);
  // std::cout << "input count hex: " << util::BytesToHex(input_count) << std::endl;
  std::cout << "input count: " << input_count_int << std::endl;
  Input* input;
  for (uint64_t i = 0; i < input_count_int; i++) {
    std::cout << "input " << i << std::endl;
    // std::cout << "here inputs" << std::endl;
    // std::cout << "iterator enter: " << iterator << std::endl;
    input = ParseInput(data, iterator);
    inputs.push_back(*input);
  }
  // std::cout << "here7" << std::endl;

  std::vector<uint8_t> output_count;
  iterator += VarInt(data, iterator, output_count);

  std::vector<Output> outputs;
  uint64_t output_count_int = util::BytesToUInt64(output_count);

  std::cout << "output count: " << output_count_int << std::endl;
  Output* output;
  for (uint64_t i = 0; i < output_count_int; i++) {
    std::cout << "output " << i << std::endl;
    output = ParseOutput(data, iterator);
    // std::cout << "onemli 1" << std::endl;
    outputs.push_back(*output);
    // std::cout << "onemli 2" << std::endl;
  }

  // std::cout << "here8" << std::endl;

  std::array<uint8_t, 4> lock_time;
  std::copy(data + iterator, data + iterator + 4, lock_time.begin());
  iterator += 4;

  iterator = 0;
  Transaction* res =
      new Transaction(version_str, input_count, inputs, output_count, outputs, lock_time);

  return res;
}

}  // namespace tx
}  // namespace bitcoin