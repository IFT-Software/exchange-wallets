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

}  // namespace script
}  // namespace bitcoin

#endif