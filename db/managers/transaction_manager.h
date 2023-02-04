#ifndef DB_MANAGERS_TRANSACTION_H
#define DB_MANAGERS_TRANSACTION_H

#include "db/manager.h"

#include "boost/json.hpp"

class DbTransactionManager : public DbManager {
 private:
 public:
  DbTransactionManager(Db* db);

  void CreateTable() override;
  json::object Insert(json::object obj) override;
  json::object Update(json::object obj) override;
  json::object Delete(json::object obj) override;
  json::array Select(json::object obj) override;
};

#endif