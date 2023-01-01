#ifndef BITCOIN_SCRIPTPUBKEY_H
#define BITCOIN_SCRIPTPUBKEY_H

#include <vector>

#include "bitcoin/address.h"
#include "bitcoin/pubkey.h"

enum LockType { p2pkh, p2sh };

class Script {
 private:
  LockType lock_type;
  std::vector<uint8_t> script;
};

/**
 * @brief Gets the locking script for the given address
 *
 * @return Script
 */
Script GetScriptForAddr(Address& addr);

/**
 * @brief Gets the p2pk locking script for the given public key
 *
 * @param pub_key
 * @return LockingScript
 */
Script GetScriptForPubKey(PubKey& pub_key);

Script GetScriptForMultiSig(int num_keys, std::vector<PubKey>& keys);

#endif