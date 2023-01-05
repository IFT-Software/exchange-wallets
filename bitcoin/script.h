#ifndef BITCOIN_SCRIPT_H
#define BITCOIN_SCRIPT_H

#include "bitcoin/pubkey.h"

namespace bitcoin {
namespace script {

enum class OPCodeType : uint8_t {
  OP_DUP = 0x76,
  OP_HASH160 = 0xa9,
  OP_EQUALVERIFY = 0x88,
  OP_CHECKSIG = 0xac
};

void GenerateP2PKHScript(PubKey& pub_key, std::array<uint8_t, 24>& res);
void GenerateP2PKHScript(std::array<uint8_t, 65>& pub_key, std::array<uint8_t, 24>& res);

// TODO:
void GenerateP2PKHScript(std::array<uint8_t, 33>& pub_key, std::array<uint8_t, 24>& res);

// TODO:
void GenerateP2PKHScript(std::array<uint8_t, 20>& pub_key_hash, std::array<uint8_t, 24>& res);

}  // namespace script
}  // namespace bitcoin

#endif