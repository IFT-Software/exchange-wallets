#include "db/objects/address.h"

DbAddress::DbAddress(uint32_t id, std::string drv_path, std::string addr, std::string addr_type,
                     std::string prv_key, std::string pub_key, uint32_t wallet_id)
    : id_(id),
      drv_path_(drv_path),
      addr_(addr),
      addr_type_(addr_type),
      prv_key_(prv_key),
      pub_key_(pub_key),
      wallet_id_(wallet_id) {}

void DbAddress::SetId(uint32_t id) { id_ = id; }
void DbAddress::SetDerivationPath(std::string drv_path) { drv_path_ = drv_path; }
void DbAddress::SetAddress(std::string addr) { addr_ = addr; }
void DbAddress::SetAddressType(std::string addr_type) { addr_type_ = addr_type; }
void DbAddress::SetPrivateKey(std::string prv_key) { prv_key_ = prv_key; }
void DbAddress::SetPublicKey(std::string pub_key) { pub_key_ = pub_key; }

void DbAddress::SetWalletId(uint32_t wallet_id) { wallet_id_ = wallet_id; }

uint32_t DbAddress::GetId() { return id_; }
std::string DbAddress::GetDerivationPath() { return drv_path_; }
std::string DbAddress::GetAddress() { return addr_; }
std::string DbAddress::GetAddressType() { return addr_type_; }
std::string DbAddress::GetPrivateKey() { return prv_key_; }
std::string DbAddress::GetPublicKey() { return pub_key_; }
uint32_t DbAddress::GetWalletId() { return wallet_id_; }