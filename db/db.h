#include <string>
#include <vector>

// enum DbType { SQLITE, POSTGRESQL };

class Db {
 protected:
  std::string db_name_;

 public:
  Db();
  Db(std::string db_name);

  void SetName(std::string db_name);
  std::string GetName();

  virtual bool Connect() = 0;
  virtual bool Disconnect() = 0;
  virtual bool IsConnected() = 0;

  virtual bool Execute(std::string query) = 0;
  virtual bool ExecuteWithResult(std::string query, void* res) = 0;
  virtual bool ExecuteTransaction(std::vector<std::string> queries) = 0;

  // virtual void SetType(DbType db_type) = 0;
  // virtual DbType GetType() = 0;
};