#include "wallet/wallet_address.h"

#include <string>

WalletAddress::WalletAddress(std::string& addr, AddrType& addr_type, std::string derivation_path)
    : Address(addr, addr_type), derivation_path_(derivation_path) {}

WalletAddress::WalletAddress(Address address, std::string derivation_path)
    : Address(address), derivation_path_(derivation_path) {}

std::string WalletAddress::GetDerivationPath() { return derivation_path_; }