#ifndef BITCOIN_CRYPTO_H
#define BITCOIN_CRYPTO_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class PubKey;
class Script;
enum class AddrType : uint16_t;

namespace bitcoin {
namespace crypto {

[[maybe_unused]] void GenerateExtPrivKey(const uint8_t* seed, uint8_t* res);
[[maybe_unused]] void GenerateExtPrivKey(std::array<uint8_t, 64>& seed,
                                         std::array<uint8_t, 64>& extended_priv_key);

[[maybe_unused]] void GeneratePrivKey(const uint8_t* seed, uint8_t* priv_key);
[[maybe_unused]] void GeneratePrivKey(std::array<uint8_t, 64>& seed,
                                      std::array<uint8_t, 32>& priv_key);

[[maybe_unused]] void GenerateChainCode(const uint8_t* seed, uint8_t* chain_code);
[[maybe_unused]] void GenerateChainCode(std::array<uint8_t, 64>& seed,
                                        std::array<uint8_t, 32>& chain_code);

[[maybe_unused]] void GenerateChecksum(const uint8_t* data, size_t len, uint8_t* res);

template <size_t N>
[[maybe_unused]] void GenerateChecksum(std::array<uint8_t, N>& data, std::array<uint8_t, 4>& res) {
  GenerateChecksum(data.begin(), data.size(), res.begin());
}

[[maybe_unused]] std::string GenerateWIF(const int net, const uint8_t* priv_key);
[[maybe_unused]] std::string GenerateWIF(const int net, std::array<uint8_t, 32>& priv_key);

[[maybe_unused]] void GeneratePubKey(std::array<uint8_t, 32>& priv_key,
                                     std::array<uint8_t, 33>& pub_key);

[[maybe_unused]] void GeneratePubKeyUncomp(std::array<uint8_t, 32>& priv_key,
                                           std::array<uint8_t, 65>& pub_key);

[[maybe_unused]] void GeneratePubKeyHash(const uint8_t* pub_key, size_t len, uint8_t* res);

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 33>& pub_key,
                                         std::array<uint8_t, 20>& res);

[[maybe_unused]] void GeneratePubKeyHash(std::array<uint8_t, 65>& pub_key,
                                         std::array<uint8_t, 20>& res);

// // used for p2pkh
[[maybe_unused]] std::string GenerateAddressFromPubkey(PubKey& pub_key, AddrType& addr_type);

// TODO:
[[maybe_unused]] std::string GenerateAddressFromPubkey(std::array<uint8_t, 33>& pub_key);
[[maybe_unused]] std::string GenerateAddressFromPubkey(std::array<uint8_t, 65>& pub_key);

// used for p2sh
// TODO:
[[maybe_unused]] std::string GenerateAddressFromScript(Script& script);

}  // namespace crypto
}  // namespace bitcoin

#endif