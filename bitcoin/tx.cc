#include "bitcoin/tx.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "bitcoin/transaction.h"
#include "util/util.h"

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
  std::array<uint8_t, 32> prev_tx;
  std::copy(data + iterator, data + iterator + 32, prev_tx.begin());
  iterator += 32;

  std::array<uint8_t, 4> vout;
  std::copy(data + iterator, data + iterator + 4, vout.begin());
  iterator += 4;

  std::vector<uint8_t> sig_size;
  iterator += VarInt(data, iterator, sig_size);
  uint64_t sig_size_int = util::BytesToUInt64(sig_size);

  std::vector<uint8_t> sig(sig_size_int);
  std::copy(data + iterator, data + iterator + sig_size_int, sig.begin());
  iterator += sig_size_int;

  std::array<uint8_t, 4> seqeunce;
  std::copy(data + iterator, data + iterator + 4, seqeunce.begin());
  iterator += 4;

  Input* res = new Input(prev_tx, vout, sig_size, sig, seqeunce);
  return res;
}

Output* ParseOutput(uint8_t* data, uint32_t& iterator) {
  std::array<uint8_t, 8> value;
  std::copy(data + iterator, data + iterator + 8, value.begin());
  iterator += 8;

  std::vector<uint8_t> script_pubkey_size;
  iterator += VarInt(data, iterator, script_pubkey_size);
  uint64_t script_pubkey_size_int = util::BytesToUInt64(script_pubkey_size);

  std::vector<uint8_t> script_pubkey(script_pubkey_size_int);
  std::copy(data + iterator, data + iterator + script_pubkey_size_int, script_pubkey.begin());
  iterator += script_pubkey_size_int;

  Output* res = new Output(value, script_pubkey_size, script_pubkey);

  return res;
}

/**
 * @brief Advances the iterator. Since we are only using the confirmed transactions, we
 * don't need the data in the witness field, which mostly consists of the signature and the
 * public key.
 *
 * @param data
 * @param iterator
 * @param n_witness equals to the number of inputs of the transaction
 */
void ParseWitness(uint8_t* data, uint32_t& iterator, uint32_t n_witness) {
  for (uint32_t i = 0; i < n_witness; i++) {
    std::vector<uint8_t> n_items;
    iterator += VarInt(data, iterator, n_items);

    uint64_t n_items_int = util::BytesToUInt64(n_items);
    if (n_items_int == 0) {
      continue;
    }

    for (uint64_t j = 0; j < n_items_int; j++) {
      std::vector<uint8_t> item_size;
      iterator += VarInt(data, iterator, item_size);
      uint64_t item_size_int = util::BytesToUInt64(item_size);
      iterator += item_size_int;
    }
  }
}

Transaction* ParseTransaction(uint8_t* data) {
  uint32_t iterator = 0;
  bool is_segwit = false;

  std::array<uint8_t, 4> version;
  std::copy(data, data + 4, version.begin());
  std::string version_str = util::BytesToHex(version);

  iterator += 4;

  std::array<uint8_t, 2> flag;
  std::copy(data + iterator, data + iterator + 2, flag.begin());

  uint32_t flag_int = util::BytesToUInt16(flag);

  if (flag_int == 1) {
    iterator += 2;
    is_segwit = true;
  }

  std::vector<uint8_t> input_count;
  iterator += VarInt(data, iterator, input_count);

  std::vector<Input> inputs;
  uint64_t input_count_int = util::BytesToUInt64(input_count);
  Input* input;
  for (uint64_t i = 0; i < input_count_int; i++) {
    input = ParseInput(data, iterator);
    inputs.push_back(*input);
  }

  std::vector<uint8_t> output_count;
  iterator += VarInt(data, iterator, output_count);

  std::vector<Output> outputs;
  uint64_t output_count_int = util::BytesToUInt64(output_count);

  Output* output;
  for (uint64_t i = 0; i < output_count_int; i++) {
    output = ParseOutput(data, iterator);
    outputs.push_back(*output);
  }

  ParseWitness(data, iterator, input_count_int);

  std::array<uint8_t, 4> lock_time;
  std::copy(data + iterator, data + iterator + 4, lock_time.begin());
  iterator += 4;

  iterator = 0;
  Transaction* res = new Transaction(version_str, input_count, inputs, output_count, outputs,
                                     lock_time, is_segwit);

  return res;
}

bool IsSegwit(uint8_t* data) {
  std::array<uint8_t, 2> flag;
  std::copy(data + 4, data + 6, flag.begin());

  uint32_t flag_int = util::BytesToUInt16(flag);

  if (flag_int == 1) {
    return true;
  }
  return false;
}

}  // namespace tx
}  // namespace bitcoin