#include "db/manager.h"

#include "boost/json.hpp"

namespace json = boost::json;

class DbWalletManager : public DbManager {
 private:
 public:
  DbWalletManager(Db* db);

  void CreateTable() override;
  json::object Insert(json::object obj) override;
  json::object Update(json::object obj) override;
  json::object Delete(json::object obj) override;
  json::object Select(json::object obj) override;
};