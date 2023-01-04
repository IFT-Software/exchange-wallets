#ifndef BITCOIN_PRIVKEY_H
#define BITCOIN_PRIVKEY_H

#include <array>
#include <cstdint>

#include "bitcoin/pubkey.h"
#include "bitcoin/signature.h"
#include "util/util.h"

class PrivKey {
 private:
  std::array<uint8_t, 32> priv_key_;

 public:
  PrivKey(std::array<uint8_t, 32>& priv_key);

  // todo
  bool IsValid();

  uint32_t size() const;
  const uint8_t* data() const;
  const uint8_t* begin() const;
  const uint8_t* end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex() const;
  std::string bin() const;

  /**
   * @brief Verifies if this private key corresponds to the given public key.
   *
   * @param pub_key The public key to be verified.
   * @return true
   * @return false
   */
  bool VerifyPubKey(PubKey& pub_key);

  // todo
  ScriptSig Sign();
};

typedef std::array<uint8_t, 32> ChainCode;

class ExtPrivKey {
 private:
  std::array<uint8_t, 32> priv_key_;
  std::array<uint8_t, 32> chain_code_;

 public:
  ExtPrivKey(PrivKey priv_key, ChainCode chain_code);
  ExtPrivKey(std::array<uint8_t, 32> priv_key_, std::array<uint8_t, 32> chain_code_);
  ExtPrivKey(std::array<uint8_t, 64> ext_priv_key);

  // todo: write these functions
  bool IsValid();

  uint32_t size() const;
  const uint8_t* data() const;
  const uint8_t* begin() const;
  const uint8_t* end() const;
  const uint8_t& operator[](uint32_t pos) const;

  std::string hex() const;
  std::string bin() const;
};

#endif