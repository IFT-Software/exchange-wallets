#include <string>

#include "bitcoin/address.h"

class WalletAddress : public Address {
 private:
  std::string derivation_path_;
  std::string prv_key_;
  std::string pub_key_;

 public:
  // WalletAddress(std::string& addr, AddrType& addr_type, std::string derivation_path);
  WalletAddress(Address address, std::string derivation_path);

  std::string GetDerivationPath();
  std::string GetPrivateKey();
  std::string GetPublicKey();
};