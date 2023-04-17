#include "db/managers/transaction_manager.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "boost/json.hpp"
#include "util/util.h"

namespace json = boost::json;

DbTransactionManager::DbTransactionManager(Db* db) : DbManager(db, "Transaction") {}

std::string DbTransactionManager::BuildSetSubQuery(json::object obj) {
  std::vector<std::string> keys_values;
  for (auto value : obj) {
    std::string key_value = absl::StrCat(value.key_c_str(), " = ");

    if (value.key() == "txid") {
      absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));
    } else if (value.key() == "version") {
      absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));
    } else if (value.key() == "inputs") {
      absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));
      absl::StrAppend(&key_value, "::input[]");
    } else if (value.key() == "outputs") {
      absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));
      absl::StrAppend(&key_value, "::output[]");
    } else if (value.key() == "lock_time") {
      absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));
    } else {
      std::string error_msg =
          absl::StrCat("Invalid field '", value.key_c_str(), "' at json object.");
      throw std::invalid_argument(error_msg);
    }

    keys_values.push_back(key_value);
  }
  return absl::StrCat("SET ", absl::StrJoin(keys_values, ","));
}

std::string DbTransactionManager::BuildInsertQuery(json::object obj, std::string& table_name) {
  if (obj["data"].is_null()) {
    throw std::invalid_argument("Insertion Error: query object should have 'data' field");
  }

  std::vector<std::string> insert_keys;
  std::vector<std::string> insert_values;

  for (auto value : obj["data"].as_object()) {
    insert_keys.push_back(value.key());

    std::string str_value = "";
    if (value.key() == "txid") {
      str_value = util::JsonValueToSQLFormattedStr(value.value());
    } else if (value.key() == "version") {
      str_value = util::JsonValueToSQLFormattedStr(value.value());
    } else if (value.key() == "inputs") {
      str_value = util::JsonValueToSQLFormattedStr(value.value());
      absl::StrAppend(&str_value, "::input[]");
    } else if (value.key() == "outputs") {
      str_value = util::JsonValueToSQLFormattedStr(value.value());
      absl::StrAppend(&str_value, "::output[]");
    } else if (value.key() == "lock_time") {
      str_value = util::JsonValueToSQLFormattedStr(value.value());
    } else {
      std::string error_msg =
          absl::StrCat("Invalid field '", value.key_c_str(), "' at json object.");
      throw std::invalid_argument(error_msg);
    }

    insert_values.push_back(str_value);
  }

  std::string joined_insert_keys = absl::StrJoin(insert_keys, ",");
  std::string joined_insert_values = absl::StrJoin(insert_values, ",");

  std::string query = absl::StrCat("INSERT INTO ", table_name, " (", joined_insert_keys,
                                   ") VALUES (", joined_insert_values, ")");

  // TODO: Here is just made for postgresql but this should be applicable for all supported dbs.
  if (!obj["select"].is_null()) {
    absl::StrAppend(&query, " ", BuildReturningSubQuery(obj["select"].as_object()));
  }

  absl::StrAppend(&query, ";");

  return query;
}

std::string DbTransactionManager::BuildUpdateQuery(json::object obj, std::string& table_name) {
  if (obj["data"].is_null()) {
    throw std::invalid_argument("Update Error: query object should have 'data' field");
  }

  if (obj["where"].is_null()) {
    throw std::invalid_argument("Update Error: query object should have 'where' field");
  }

  std::string set_sub_query = BuildSetSubQuery(obj["data"].as_object());
  std::string where_sub_query = BuildWhereSubQuery(obj["where"].as_object());

  std::string query = absl::StrCat("UPDATE ", table_name, " ", set_sub_query, " ", where_sub_query);

  // TODO: Here is just made for postgresql but this should be applicable for all supported dbs.
  if (!obj["select"].is_null()) {
    absl::StrAppend(&query, " ", BuildReturningSubQuery(obj["select"].as_object()));
  }

  absl::StrAppend(&query, ";");
  return query;
}

json::object DbTransactionManager::GetUniqueSelectQueryResult(json::object& obj,
                                                              const pqxx::row& pq_res_row) {
  json::object res_obj;
  for (auto value : obj["select"].as_object()) {
    if (value.value().as_bool()) {
      if (!pq_res_row[value.key_c_str()].is_null()) {
        if (value.key() == "txid") {
          res_obj[value.key_c_str()] = pq_res_row[value.key_c_str()].as<std::string>();
        } else if (value.key() == "version") {
          res_obj[value.key_c_str()] = pq_res_row[value.key_c_str()].as<uint32_t>();
        } else if (value.key() == "inputs") {
          auto arr = pq_res_row[value.key_c_str()].as_array();
          std::pair<pqxx::array_parser::juncture, std::string> elem = arr.get_next();

          json::array input_json_arr;
          while (elem.first != pqxx::array_parser::juncture::done) {
            if (elem.second.size()) {
              std::vector<std::string> input_elem =
                  absl::StrSplit(elem.second.substr(1, elem.second.size() - 2), ",");
              input_json_arr.push_back({{"txid", input_elem[0]},
                                        {"vout", std::stoul(input_elem[1])},
                                        {"address", input_elem[2]}});
            }
            elem = arr.get_next();
          }

          res_obj[value.key_c_str()] = input_json_arr;
        } else if (value.key() == "outputs") {
          auto arr = pq_res_row[value.key_c_str()].as_array();
          std::pair<pqxx::array_parser::juncture, std::string> elem = arr.get_next();

          json::array output_json_arr;
          while (elem.first != pqxx::array_parser::juncture::done) {
            if (elem.second.size()) {
              std::vector<std::string> output_elem =
                  absl::StrSplit(elem.second.substr(1, elem.second.size() - 2), ",");
              output_json_arr.push_back(
                  {{"address", output_elem[0]}, {"value", std::stoul(output_elem[1])}});
            }
            elem = arr.get_next();
          }

          res_obj[value.key_c_str()] = output_json_arr;
        } else if (value.key() == "lock_time") {
          res_obj[value.key_c_str()] = pq_res_row[value.key_c_str()].as<uint32_t>();
        }

      } else {
        res_obj[value.key_c_str()] = nullptr;
      }
    }
  }
  return res_obj;
}

void DbTransactionManager::CreateTable() {
  std::string query = "CREATE TABLE IF NOT EXISTS " + table_name_ +
                      " (txid VARCHAR(255) PRIMARY "
                      "KEY, version INTEGER, inputs Input[], outputs Output[], lock_time "
                      "INTEGER)";
  db_->Execute(query);
}

json::object DbTransactionManager::Insert(json::object obj) {
  std::string query = BuildInsertQuery(obj, table_name_);

  // std::cout << "DEBUG(query): " << query << std::endl;

  json::object res_obj;
  if (obj["select"].is_null()) {
    db_->Execute(query);
  } else {
    std::any res;
    try {
      db_->ExecuteWithResult(query, res);
    } catch (const pqxx::unique_violation& e) {
      std::cerr << e.what() << std::endl;
      throw e;
    }
    pqxx::result pq_res = std::any_cast<pqxx::result>(res);
    if (pq_res.size() > 0) {
      res_obj = GetUniqueSelectQueryResult(obj, pq_res[0]);
    }
  }
  // std::cout << "returning wout exp" << std::endl;
  return res_obj;
}

json::object DbTransactionManager::Update(json::object obj) {
  std::string query = BuildUpdateQuery(obj, table_name_);

  std::cout << "DEBUG(query): " << query << std::endl;

  json::object res_obj;
  if (obj["select"].is_null()) {
    db_->Execute(query);
  } else {
    std::any res;
    db_->ExecuteWithResult(query, res);

    pqxx::result pq_res = std::any_cast<pqxx::result>(res);

    if (pq_res.size() > 0) {
      res_obj = GetUniqueSelectQueryResult(obj, pq_res[0]);
    }
  }
  return res_obj;
}

json::object DbTransactionManager::Delete(json::object obj) {
  std::string query = BuildDeleteQuery(obj, table_name_);

  std::cout << "DEBUG(query): " << query << std::endl;

  json::object res_obj;
  if (obj["select"].is_null()) {
    db_->Execute(query);
  } else {
    std::any res;
    db_->ExecuteWithResult(query, res);

    pqxx::result pq_res = std::any_cast<pqxx::result>(res);

    if (pq_res.size() > 0) {
      res_obj = GetUniqueSelectQueryResult(obj, pq_res[0]);
    }
  }
  return res_obj;
}

json::array DbTransactionManager::Select(json::object obj) {
  std::string query = BuildSelectQuery(obj, table_name_);

  // std::cout << "DEBUG(query): " << query << std::endl;

  std::any res;
  db_->ExecuteWithResult(query, res);

  pqxx::result pq_res = std::any_cast<pqxx::result>(res);

  json::array res_arr;

  if (pq_res.size() > 0) {
    for (pqxx::row pq_res_row : pq_res) {
      json::object obj_elem = GetUniqueSelectQueryResult(obj, pq_res_row);
      res_arr.emplace_back(obj_elem);
    }
  }

  return res_arr;
}