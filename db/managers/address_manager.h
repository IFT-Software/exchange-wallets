#ifndef DB_MANAGERS_ADDRESS_H
#define DB_MANAGERS_ADDRESS_H

#include "db/manager.h"

#include "boost/json.hpp"

class DbAddressManager : public DbManager {
 private:
 public:
  DbAddressManager(Db* db);

  void CreateTable() override;
  json::object Insert(json::object obj) override;
  json::object Update(json::object obj) override;
  json::object Delete(json::object obj) override;
  json::object Select(json::object obj) override;
};

#endif