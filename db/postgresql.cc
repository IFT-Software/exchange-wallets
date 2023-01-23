#include "db/postgresql.h"

#include <exception>
#include <iostream>
#include <string>

#include "pqxx/pqxx"

Postgresql::Postgresql(std::string db_name, std::string host, uint32_t port, std::string user,
                       std::string password)
    : Db(db_name), host_(host), port_(port), user_(user), password_(password) {
  Connect();
}

Postgresql::~Postgresql() { Disconnect(); }

bool Postgresql::Connect() {
  try {
    std::string conn_str = "postgresql://" + user_ + ":" + password_ + "@" + host_ + ":" +
                           std::to_string(port_) + "/" + db_name_;
    // std::cout << conn_str << std::endl;
    connection_ = new pqxx::connection(conn_str.c_str());

    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::Disconnect() {
  try {
    connection_->close();
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::IsConnected() { return connection_->is_open(); }

bool Postgresql::Execute(std::string& query) {
  try {
    pqxx::nontransaction work(*connection_);
    work.exec(query);
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::ExecuteWithResult(std::string& query, std::any& res) {
  try {
    pqxx::nontransaction work(*connection_);
    pqxx::result result = work.exec(query);
    res = result;
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool Postgresql::ExecuteTransaction(std::vector<std::string>& queries) {
  try {
    pqxx::work work(*connection_);
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