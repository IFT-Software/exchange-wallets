#include "db/managers/wallet_manager.h"
#include <iostream>
#include <tuple>

#include "boost/json.hpp"

namespace json = boost::json;

DbWalletManager::DbWalletManager(Db* db) : DbManager(db, "Wallet") {}

void DbWalletManager::CreateTable() {
  std::string query =
      "CREATE TABLE IF NOT EXISTS " + table_name_ +
      " (id SERIAL PRIMARY KEY, name VARCHAR(255), seed VARCHAR(255), coin VARCHAR(255));";
  db_->Execute(query);
}

json::object DbWalletManager::Insert(json::object obj) {
  std::string query = BuildInsertQuery(obj, table_name_);

  // std::cout << "DEBUG(query): " << query << std::endl;

  json::object res_obj;
  if (obj["select"].is_null()) {
    db_->Execute(query);
  } else {
    std::any res;
    db_->ExecuteWithResult(query, res);

    pqxx::result pq_res = std::any_cast<pqxx::result>(res);

    if (pq_res.size() > 0) {
      for (auto value : obj["select"].as_object()) {
        if (value.value().as_bool()) {
          if (!pq_res[0][value.key_c_str()].is_null()) {
            res_obj[value.key_c_str()] = pq_res[0][value.key_c_str()].c_str();
          } else {
            res_obj[value.key_c_str()] = nullptr;
          }
        }
      }
    }
  }
  return res_obj;
}

json::object DbWalletManager::Update(json::object obj) {
  std::string query = "UPDATE " + table_name_ + " SET name = '" + obj["name"].as_string().c_str() +
                      "', seed = '" + obj["seed"].as_string().c_str() + "', coin = '" +
                      obj["coin"].as_string().c_str() +
                      "' WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";
  db_->Execute(query);
  return json::object();
}

json::object DbWalletManager::Delete(json::object obj) {
  std::string query =
      "DELETE FROM " + table_name_ + " WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";
  db_->Execute(query);
  return json::object();
}

json::object DbWalletManager::Select(json::object obj) {
  std::string query =
      "SELECT * FROM " + table_name_ + " WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";

  std::any res;
  db_->ExecuteWithResult(query, res);

  pqxx::result pq_res = std::any_cast<pqxx::result>(res);

  json::object res_obj;
  if (pq_res.size() > 0) {
    if (!pq_res[0]["name"].is_null()) {
      res_obj["name"] = pq_res[0]["name"].as<std::string>();
    } else {
      res_obj["name"] = nullptr;
    }

    if (!pq_res[0]["seed"].is_null()) {
      res_obj["seed"] = pq_res[0]["seed"].as<std::string>();
    } else {
      res_obj["seed"] = nullptr;
    }

    if (!pq_res[0]["coin"].is_null()) {
      res_obj["coin"] = pq_res[0]["coin"].as<std::string>();
    } else {
      res_obj["coin"] = nullptr;
    }
  }

  return res_obj;
}