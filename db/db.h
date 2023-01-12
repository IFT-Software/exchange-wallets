#include <string>

enum DbType { SQLITE, POSTGRESQL };

class Db {
 private:
  std::string db_name_;

 public:
  Db();
  Db(std::string db_name);

  void SetName(std::string db_name);
  std::string GetName();

  virtual bool Connect() = 0;
  virtual bool Disconnect() = 0;
  virtual bool IsConnected() = 0;

  virtual void SetType(DbType db_type) = 0;
  virtual DbType GetType() = 0;
};