#include <string>

#include "bitcoin/address.h"

class WalletAddress : public Address {
 private:
  std::string derivation_path_;

 public:
  WalletAddress(std::string& addr, AddrType& addr_type, std::string derivation_path);
  WalletAddress(Address address, std::string derivation_path);

  std::string GetDerivationPath();
};