//#include "db/db_object.h"

#include <cstdint>
#include <string>

class DbAddress {
 private:
  uint32_t id_;
  std::string drv_path_;
  std::string addr_;
  std::string addr_type_;
  std::string prv_key_;
  std::string pub_key_;
  uint32_t wallet_id_;

 public:
  DbAddress(uint32_t id, std::string drv_path, std::string addr, std::string addr_type,
            std::string prv_key, std::string pub_key, uint32_t wallet_id);

  void SetId(uint32_t id);
  void SetDerivationPath(std::string drv_path);
  void SetAddress(std::string addr);
  void SetAddressType(std::string addr_type);
  void SetPrivateKey(std::string prv_key);
  void SetPublicKey(std::string pub_key);

  void SetWalletId(uint32_t wallet_id);

  uint32_t GetId();
  std::string GetDerivationPath();
  std::string GetAddress();
  std::string GetAddressType();
  std::string GetPrivateKey();
  std::string GetPublicKey();
  uint32_t GetWalletId();
};
