#include "bitcoin/script.h"

#include "bitcoin/crypto.h"
#include "util/util.h"

namespace bitcoin {
namespace script {

void GenerateP2PKHScript(PubKey& pub_key, std::array<uint8_t, 24>& res) {
  // TODO: MAKE THIS 25 BYTES AND INCLUDE length!!!
  std::array<uint8_t, 20> pub_key_hash = pub_key.GetHash160();
  std::cout << "hash160: " << util::BytesToHex(pub_key_hash) << std::endl;
  res[0] = (uint8_t)OPCodeType::OP_DUP;
  res[1] = (uint8_t)OPCodeType::OP_HASH160;
  res[22] = (uint8_t)OPCodeType::OP_EQUALVERIFY;
  res[23] = (uint8_t)OPCodeType::OP_CHECKSIG;

  std::copy(pub_key_hash.begin(), pub_key_hash.end(), res.begin() + 2);
}

void GenerateP2PKHScript(std::array<uint8_t, 65>& pub_key, std::array<uint8_t, 24>& res) {
  // try to construct a PubKey object, if can't construct, invalid key (raise constructor's
  // exception)
  PubKey pub_key_ = PubKey(pub_key);
  GenerateP2PKHScript(pub_key_, res);
}

void GenerateP2PKHScript(std::array<uint8_t, 33>& pub_key, std::array<uint8_t, 24>& res) {}

void GenerateP2PKHScript(std::array<uint8_t, 20>& pub_key_hash, std::array<uint8_t, 24>& res){};

}  // namespace script
}  // namespace bitcoin