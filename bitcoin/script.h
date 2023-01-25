// this namespace provides the necessary transaction functions between addresses and scriptpubkeys.

#ifndef BITCOIN_SCRIPT_H
#define BITCOIN_SCRIPT_H

#include "bitcoin/address.h"
#include "bitcoin/pubkey.h"

namespace bitcoin {
namespace script {

enum class OPCodeType : uint8_t {
  OP_DUP = 0x76,
  OP_HASH160 = 0xa9,
  OP_EQUALVERIFY = 0x88,
  OP_CHECKSIG = 0xac
};

/**
 * @brief Generates a p2pkh script from the public key object @param pub_key and writes it into the
 * result array @param res.
 *
 * @param pub_key a public key with type PubKey
 * @param res resulting 24 byte array
 */
void GenerateP2PKHScript(PubKey& pub_key, std::array<uint8_t, 24>& res);

/**
 * @brief Generates a p2pkh script for an address @param address and writes it into the result array
 * @param res.
 *
 * @param address
 * @param res
 */
void GenerateP2PKHScript(Address& address, std::array<uint8_t, 24>& res);

// TODO: implement the other script types

void MatchScriptToOutputType();

void ExtractAddress(const Script& scriptpubkey);

}  // namespace script
}  // namespace bitcoin

#endif