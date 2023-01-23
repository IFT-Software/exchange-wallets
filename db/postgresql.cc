#include "db/postgresql.h"

#include <iostream>
#include <string>

#include "pqxx/pqxx"

Postgresql::Postgresql() : Db(), host_("localhost"), user_("postgres"), password_("") {
  if (Connect()) {
    std::cout << "Database connection successfull." << std::endl;
  } else {
    std::cout << "Database connection failed." << std::endl;
  }
}

Postgresql::Postgresql(std::string db_name, std::string host, std::string user,
                       std::string password)
    : Db(db_name), host_(host), user_(user), password_(password) {
  Connect();
}

Postgresql::~Postgresql() { Disconnect(); }

std::string Postgresql::GetHost() { return host_; }

std::string Postgresql::GetUser() { return user_; }

std::string Postgresql::GetPassword() { return password_; }

bool Postgresql::Connect() {
  try {
    connection_ = pqxx::connection(("host=" + GetHost() + " dbname=" + GetDbName() +
                                    " user=" + GetUser() + " password=" + GetPassword())
                                       .c_str());
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool Postgresql::Disconnect() { connection_.close(); }

bool Postgresql::IsConnected() { return connection_.is_open(); }

bool Postgresql::Execute(std::string query) {
  try {
    pqxx::work work(connection_);
    work.exec(query);
    work.commit();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

pqxx::result Postgresql::ExecuteWithResult(std::string query) {
  pqxx::result result;
  try {
    pqxx::nontransaction work(connection_);
    result = work.exec(query);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return result;
}

// std::string Postgresql::GetLastErrorMessage() { return connection.errormessage(); }
