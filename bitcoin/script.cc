#include "script.h"

#include "crypto.h"

namespace bitcoin {
namespace script {

void GenerateP2PKHScript(PubKey& pub_key, std::array<uint8_t, 24>& res) {
  std::array<uint8_t, 20> pub_key_hash = pub_key.GetHash160();
  res[0] = OP_DUP;
  res[1] = OP_HASH160;
  res[22] = OP_EQUALVERIFY;
  res[23] = OP_CHECKSIG;

  std::copy(pub_key_hash.begin(), pub_key_hash.end(), res.begin() + 2);
}

void GenerateP2PKHScript(std::array<uint8_t, 65>& pub_key, std::array<uint8_t, 24>& res) {
  // try to construct a PubKey object, if can't construct, invalid key (raise constructor's
  // exception)
  PubKey pub_key_ = PubKey(pub_key);
  GenerateP2PKHScript(pub_key_, res);
}

void GenerateP2PKHScript(std::array<uint8_t, 33>& pub_key, std::array<uint8_t, 24>& res) {}
void GenerateP2PKHScript(std::array<uint8_t, 20>& pub_key_hash, std::array<uint8_t, 24>& res);

}  // namespace script
}  // namespace bitcoin
