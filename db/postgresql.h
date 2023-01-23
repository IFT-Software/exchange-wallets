#include "db/db.h"

#include <string>

#include "pqxx/pqxx"

class Postgresql : public Db {
 private:
  std::string host_;
  std::string user_;
  std::string password_;

  pqxx::connection connection_;

 public:
  Postgresql();
  Postgresql(std::string db_name, std::string host, std::string user, std::string password);

  ~Postgresql();

  bool Connect() override;
  bool Disconnect() override;
  bool IsConnected() override;

  bool Execute(std::string query) override;
  bool ExecuteWithResult(std::string query, void* res) override;
  bool ExecuteTransaction(std::vector<std::string> queries) override;
};