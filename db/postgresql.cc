#include "db/postgresql.h"

#include <exception>
#include <iostream>
#include <string>

#include "pqxx/pqxx"

Postgresql::Postgresql(std::string db_name, std::string host, std::string user,
                       std::string password)
    : Db(db_name), host_(host), user_(user), password_(password) {
  Connect();
}

Postgresql::~Postgresql() { Disconnect(); }

bool Postgresql::Connect() {
  try {
    connection_ = pqxx::connection(
        ("host=" + host_ + " dbname=" + db_name_ + " user=" + user_ + " password=" + password_)
            .c_str());
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::Disconnect() {
  try {
    connection_.close();
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::IsConnected() { return connection_.is_open(); }

bool Postgresql::Execute(std::string query) {
  try {
    pqxx::nontransaction work(connection_);
    work.exec(query);
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::ExecuteWithResult(std::string query, void* res) {
  try {
    pqxx::nontransaction work(connection_);
    pqxx::result result = work.exec(query);
    res = (void*)&result;
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::ExecuteTransaction(std::vector<std::string> queries) {
  try {
    pqxx::work work(connection_);
    for (auto query : queries) {
      work.exec(query);
    }
    work.commit();
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}