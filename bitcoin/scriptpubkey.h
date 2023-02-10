#ifndef BITCOIN_SCRIPTPUBKEY_H
#define BITCOIN_SCRIPTPUBKEY_H

#include <cstdint>
#include <string>
#include <vector>

// types are from https://github.com/bitcoin/bitcoin/blob/master/src/script/standard.cpp#L91
// TODO: change this to output type and use this instead of AddrType
enum OutputType {
  P2PKH,
  P2PK,
  P2SH,
  MULTISIG,
  WITNESS_V0_KEYHASH,
  WITNESS_V0_SCRIPTHASH,
  NULL_DATA,
  UNKNOWN,
  // WITNESS_V1_TAPROOT,
  // WITNESS_UNKNOWN,
  // NONSTANDARD
};

class Script {
 private:
  OutputType out_type_;
  std::vector<uint8_t> script_;

 public:
  // TODO: constructor without output type
  Script(std::vector<uint8_t> script, OutputType out_type);

  bool IsValid(std::vector<uint8_t> script, OutputType out_type);

  OutputType GetOutputType();

  uint32_t size() const;
  std::vector<uint8_t> data() const;
  std::vector<uint8_t>::const_iterator begin() const;
  std::vector<uint8_t>::const_iterator end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex();
  std::string bin();
};

#endif