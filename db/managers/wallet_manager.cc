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
  std::string query = BuildUpdateQuery(obj, table_name_);

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

json::object DbWalletManager::Delete(json::object obj) {
  std::string query = BuildDeleteQuery(obj, table_name_);

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

json::array DbWalletManager::Select(json::object obj) {
  std::string query = BuildSelectQuery(obj, table_name_);

  std::cout << "DEBUG(query): " << query << std::endl;

  std::any res;
  db_->ExecuteWithResult(query, res);

  pqxx::result pq_res = std::any_cast<pqxx::result>(res);

  json::array res_arr;

  if (pq_res.size() > 0) {
    for (auto pq_res_elem : pq_res) {
      json::object obj_elem;
      for (auto value : obj["select"].as_object()) {
        if (value.value().as_bool()) {
          if (!pq_res_elem[value.key_c_str()].is_null()) {
            obj_elem[value.key_c_str()] = pq_res_elem[value.key_c_str()].c_str();
          } else {
            obj_elem[value.key_c_str()] = nullptr;
          }
        }
      }
      res_arr.emplace_back(obj_elem);
    }
  }

  return res_arr;
}