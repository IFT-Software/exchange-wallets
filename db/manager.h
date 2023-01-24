#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>

#include "boost/json.hpp"
#include "pqxx/pqxx"

#include "db/db.h"

namespace json = boost::json;

class DbManager {
 protected:
  Db* db_;
  std::string table_name_;

  DbManager(Db* db, std::string table_name);

 public:
  virtual ~DbManager() = default;
  virtual void CreateTable() = 0;
  virtual json::object Insert(json::object obj) = 0;
  virtual json::object Update(json::object obj) = 0;
  virtual json::object Delete(json::object obj) = 0;
  virtual json::object Select(json::object obj) = 0;

  std::string BuildInsertQuery(json::object obj, std::string& table_name);
  std::string BuildUpdateQuery(json::object obj, std::string& table_name);
  std::string BuildDeleteQuery(json::object obj, std::string& table_name);
  std::string BuildSelectQuery(json::object obj, std::string& table_name);
};

#endif