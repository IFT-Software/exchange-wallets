#include "db/db.h"

#include <string>

#include "pqxx/pqxx"

class Postgresql : public Db {
 private:
  pqxx::connection connection_;

 public:
  Postgresql();
  Postgresql(std::string db_name, std::string host, std::string user, std::string password);

  ~Postgresql();
};