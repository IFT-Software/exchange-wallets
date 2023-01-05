#ifndef BITCOIN_PRIVKEY_H
#define BITCOIN_PRIVKEY_H

#include <array>
#include <cstdint>

#include "bitcoin/crypto.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/signature.h"
#include "util/util.h"

typedef std::array<uint8_t, 32> ChainCode;

class PrivKey {
 private:
  std::array<uint8_t, 32> priv_key_;

 public:
  // todo: empty constructor?
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
   * @brief Gets the corresponding compressed public key of this private key.
   *
   * @return PubKey*
   */
  PubKey GetPubKey();

  // TODO:
  /**
   * @brief Verifies if this private key corresponds to the given public key.
   *
   * @param pub_key The public key to be verified.
   * @return true
   * @return false
   */
  bool VerifyPubKey(PubKey& pub_key);

  // TODO:
  ScriptSig Sign();

  /**
   * @brief Derives a BIP32 child key, returns true if the derivation was successful.
   *
   * @param child_priv The array to write the child private key.
   * @param child_chain The array to write the  child chain code.
   * @param nChild The index of the child that will be derived.
   * @param chain_code The chain code of this private key.
   * @return true
   * @return false
   */
  bool DeriveNormalChild(uint32_t n_child, ChainCode& chain_code,
                         std::array<uint8_t, 32>& child_priv, ChainCode& child_chain);

  bool DeriveNormalChild(uint32_t n_child, ChainCode& chain_code, uint8_t* child_priv,
                         uint8_t* child_chain);
};

// used in BIP32
class ExtPrivKey {
 private:
  std::array<uint8_t, 32> priv_key_;
  std::array<uint8_t, 32> chain_code_;

  // unsigned int depth;
  // unsigned int child;

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

  std::array<uint8_t, 32> GetPrivKey() { return priv_key_; }

  /**
   * @brief Derives a BIP32 child key, returns true if the derivation was successful.
   *
   * @param child_ext_key The 64-byte array to write the extended key of the child.
   * @param nChild The index of the child that will be derived.
   * @return true
   * @return false
   */
  bool DeriveNormalChild(uint32_t n_child, std::array<uint8_t, 64>& child_ext_key);
};

#endif