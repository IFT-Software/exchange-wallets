#include <vector>

#include "pubkey.h"

enum LockType { p2pkh, p2sh };

class LockingScript {
 private:
  LockType lock_type;
  std::vector<uint8_t> script;
};

/**
 * @brief Gets the locking script for the given address
 *
 * @return Script
 */
LockingScript GetScriptForAddr(Address& addr);

/**
 * @brief Gets the p2pk locking script for the given public key
 *
 * @param pub_key
 * @return LockingScript
 */
LockingScript GetScriptForPubKey(PubKey& pub_key);

LockingScript GetScriptForMultiSig(int num_keys, std::vector<PubKey>& keys);