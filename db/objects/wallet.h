#include <string>

class DbWallet {
 private:
  uint32_t id_;

  std::string name_;
  std::string seed_;
  std::string coin_;

 public:
  DbWallet(uint32_t id, std::string name, std::string seed, std::string coin);

  void SetId(uint32_t id);
  void SetName(std::string name);
  void SetSeed(std::string seed);
  void SetCoin(std::string coin);

  uint32_t GetId();
  std::string GetName();
  std::string GetSeed();
  std::string GetCoin();
};