#include "db/manager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "absl/strings/str_join.h"
#include "boost/json.hpp"
#include "pqxx/pqxx"

#include "db/db.h"

namespace json = boost::json;

struct SqlJSONFormatter {
  void operator()(std::string* out, json::value value) const {
    if (value.is_string()) {
      absl::StrAppend(out, absl::StrCat("'", value.get_string().c_str(), "'"));
    } else if (value.is_bool()) {
      absl::StrAppend(out, value.get_bool());
    } else if (value.is_double()) {
      absl::StrAppend(out, value.get_double());
    } else if (value.is_int64()) {
      absl::StrAppend(out, value.get_int64());
    } else if (value.is_uint64()) {
      absl::StrAppend(out, value.get_uint64());
    }
  }
};

DbManager::DbManager(Db* db, std::string table_name) : db_(db), table_name_(table_name) {}

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
    std::vector<std::string> returning_keys;

    for (auto value : obj["select"].as_object()) {
      if (value.value().as_bool()) {
        returning_keys.push_back(value.key());
      }
    }

    std::string joined_returning_keys = absl::StrJoin(returning_keys, ",");

    absl::StrAppend(&query, " RETURNING ", joined_returning_keys);
  }

  absl::StrAppend(&query, ";");

  return query;
}

std::string DbManager::BuildUpdateQuery(json::object obj, std::string& table_name) {}

std::string DbManager::BuildDeleteQuery(json::object obj, std::string& table_name) {}

std::string DbManager::BuildSelectQuery(json::object obj, std::string& table_name) {}