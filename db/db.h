#ifndef DB_DB_H
#define DB_DB_H

#include <any>
#include <string>
#include <vector>

// enum DbType { SQLITE, POSTGRESQL };

class Db {
 protected:
  std::string db_name_;

 public:
  Db();
  Db(std::string db_name);

  void SetDbName(std::string db_name);
  std::string GetDbName();

  virtual bool Connect() = 0;
  virtual bool Disconnect() = 0;
  virtual bool IsConnected() = 0;

  virtual bool Execute(std::string& query) = 0;

  /**
   * @brief Executes a native SQL Query @param query and writes the returned result to @param res.
   *
   * @param query A native SQL Query string
   * @param res 'std::any' typed result object to be written on
   * @return true
   * @return false
   */
  virtual bool ExecuteWithResult(std::string& query, std::any& res) = 0;
  virtual bool ExecuteTransaction(std::vector<std::string>& queries) = 0;

  // virtual void SetType(DbType db_type) = 0;
  // virtual DbType GetType() = 0;
};

#endif