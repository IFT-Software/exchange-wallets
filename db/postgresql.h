#ifndef DB_POSTGRESQL_H
#define DB_POSTGRESQL_H

#include "db/db.h"

#include <any>
#include <string>

#include "pqxx/pqxx"

class Postgresql : public Db {
 private:
  std::string host_;
  uint32_t port_;
  std::string user_;
  std::string password_;

  pqxx::connection* connection_;

 public:
  Postgresql();
  Postgresql(std::string db_name, std::string host, uint32_t port, std::string user,
             std::string password);

  ~Postgresql();

  bool Connect() override;
  bool Disconnect() override;
  bool IsConnected() override;

  bool Execute(std::string& query) override;
  bool ExecuteWithResult(std::string& query, std::any& res) override;
  bool ExecuteTransaction(std::vector<std::string>& queries) override;
};

#endif