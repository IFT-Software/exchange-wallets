#ifndef DB_MANAGERS_TRANSACTION_H
#define DB_MANAGERS_TRANSACTION_H

#include "db/manager.h"

#include "absl/strings/str_join.h"
#include "boost/json.hpp"
#include "util/util.h"

class DbTransactionManager : public DbManager {
 private:
  json::object GetUniqueSelectQueryResult(json::object& obj, const pqxx::row& pq_res_row);

  std::string BuildSetSubQuery(json::object obj) override;

 public:
  DbTransactionManager(Db* db);

  void CreateTable() override;
  json::object Insert(json::object obj) override;
  json::object Update(json::object obj) override;
  json::object Delete(json::object obj) override;
  json::array Select(json::object obj) override;

  std::string BuildInsertQuery(json::object obj, std::string& table_name) override;
  std::string BuildUpdateQuery(json::object obj, std::string& table_name) override;
  // std::string BuildDeleteQuery(json::object obj, std::string& table_name) override;
  // std::string BuildSelectQuery(json::object obj, std::string& table_name) override;
};

#endif