#include "db/manager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "absl/strings/str_join.h"
#include "boost/json.hpp"
#include "pqxx/pqxx"

#include "db/db.h"
#include "util/util.h"

namespace json = boost::json;

struct SqlJSONFormatter {
  void operator()(std::string* out, json::value value) const {
    absl::StrAppend(out, util::JsonValueToSQLFormattedStr(value));
  }
};

DbManager::DbManager(Db* db, std::string table_name) : db_(db), table_name_(table_name) {}

std::string DbManager::BuildSetSubQuery(json::object obj) {
  std::vector<std::string> keys_values;
  for (auto value : obj) {
    std::string key_value = absl::StrCat(value.key_c_str(), " = ");

    absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));

    keys_values.push_back(key_value);
  }
  return absl::StrCat("SET ", absl::StrJoin(keys_values, ","));
}

std::string DbManager::BuildWhereSubQuery(json::object obj) {
  std::vector<std::string> keys_values;
  for (auto value : obj) {
    std::string key_value = absl::StrCat(value.key_c_str(), " = ");

    absl::StrAppend(&key_value, util::JsonValueToSQLFormattedStr(value.value()));

    keys_values.push_back(key_value);
  }
  return absl::StrCat("WHERE ", absl::StrJoin(keys_values, ","));
}

std::string DbManager::BuildReturningSubQuery(json::object obj) {
  std::vector<std::string> keys;

  for (auto value : obj) {
    if (value.value().as_bool()) {
      keys.push_back(value.key());
    }
  }

  return absl::StrCat("RETURNING ", absl::StrJoin(keys, ","));
}

std::string DbManager::BuildSelectSubQuery(json::object obj) {
  std::vector<std::string> keys;

  for (auto value : obj) {
    if (value.value().as_bool()) {
      keys.push_back(value.key());
    }
  }

  return absl::StrCat("SELECT ", absl::StrJoin(keys, ","));
}

std::string DbManager::BuildInsertQuery(json::object obj, std::string& table_name) {
  if (obj["data"].is_null()) {
    throw std::invalid_argument("Insertion Error: query object should have 'data' field");
  }

  std::vector<std::string> insert_keys;
  std::vector<json::value> insert_values;

  for (auto value : obj["data"].as_object()) {
    insert_keys.push_back(value.key());
    insert_values.push_back(value.value());
  }

  std::string joined_insert_keys = absl::StrJoin(insert_keys, ",");
  std::string joined_insert_values = absl::StrJoin(insert_values, ",", SqlJSONFormatter());

  std::string query = absl::StrCat("INSERT INTO ", table_name, " (", joined_insert_keys,
                                   ") VALUES (", joined_insert_values, ")");

  // TODO: Here is just made for postgresql but this should be applicable for all supported dbs.
  if (!obj["select"].is_null()) {
    absl::StrAppend(&query, " ", BuildReturningSubQuery(obj["select"].as_object()));
  }

  absl::StrAppend(&query, ";");

  return query;
}

std::string DbManager::BuildUpdateQuery(json::object obj, std::string& table_name) {
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

std::string DbManager::BuildDeleteQuery(json::object obj, std::string& table_name) {
  if (obj["where"].is_null()) {
    throw std::invalid_argument("Delete Error: query object should have 'where' field");
  }

  std::string where_sub_query = BuildWhereSubQuery(obj["where"].as_object());

  std::string query = absl::StrCat("DELETE FROM ", table_name, " ", where_sub_query);

  // TODO: Here is just made for postgresql but this should be applicable for all supported dbs.
  if (!obj["select"].is_null()) {
    absl::StrAppend(&query, " ", BuildReturningSubQuery(obj["select"].as_object()));
  }

  absl::StrAppend(&query, ";");
  return query;
}

std::string DbManager::BuildSelectQuery(json::object obj, std::string& table_name) {
  if (obj["select"].is_null()) {
    throw std::invalid_argument("Select Error: query object should have 'select' field");
  }

  std::string query =
      absl::StrCat(BuildSelectSubQuery(obj["select"].as_object()), " FROM ", table_name);

  if (!obj["where"].is_null()) {
    absl::StrAppend(&query, " ", BuildWhereSubQuery(obj["where"].as_object()));
  }

  absl::StrAppend(&query, ";");

  return query;
}