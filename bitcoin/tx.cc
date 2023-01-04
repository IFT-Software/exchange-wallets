#include "bitcoin/tx.h"

#include <cstdint>
#include <string>

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

Transaction::Transaction(std::string version, std::vector<uint8_t> input_count,
                         std::vector<Input> inputs, std::vector<uint8_t> output_count,
                         std::vector<Output> outputs, std::array<uint8_t, 4> lock_time)
    : version_(version),
      input_count_(input_count),
      inputs_(inputs),
      output_count_(output_count),
      outputs_(outputs),
      lock_time_(lock_time) {}

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