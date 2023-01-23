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

  std::string GetHost();
  std::string GetUser();
  std::string GetPassword();

  bool Connect() override;
  bool Disconnect() override;
  bool IsConnected() override;

  bool Execute(std::string query) override;
  pqxx::result ExecuteWithResult(std::string query);

  void SetType(DbType db_type) override;
  DbType GetType() override;
};