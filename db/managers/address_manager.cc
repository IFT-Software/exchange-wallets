#include "db/managers/address_manager.h"

DbAddressManager::DbAddressManager(Db* db) : DbManager(db, "Address") {}

void DbAddressManager::CreateTable() {
  std::string query =
      "CREATE TABLE IF NOT EXISTS " + table_name_ +
      " (id SERIAL PRIMARY KEY, drv_path VARCHAR(255), addr VARCHAR(255), "
      "addr_type VARCHAR(255), prv_key VARCHAR(255), pub_key VARCHAR(255),wallet_id "
      "INTEGER REFERENCES Wallet(id));";
  db_->Execute(query);
}

json::object DbAddressManager::Insert(json::object obj) {
  std::string query =
      "INSERT INTO " + table_name_ +
      " (drv_path, addr, addr_type, prv_key, pub_key, wallet_id) VALUES ('" +
      obj["drv_path"].as_string().c_str() + "', '" + obj["addr"].as_string().c_str() + "', '" +
      obj["addr_type"].as_string().c_str() + "', '" + obj["prv_key"].as_string().c_str() + "', '" +
      obj["pub_key"].as_string().c_str() + "','" + std::to_string(obj["wallet_id"].as_uint64()) +
      "');";

  db_->Execute(query);

  return json::object();
}

json::object DbAddressManager::Update(json::object obj) {
  std::string query =
      "UPDATE " + table_name_ + " SET drv_path = '" + obj["drv_path"].as_string().c_str() +
      "', addr = '" + obj["addr"].as_string().c_str() + "', addr_type = '" +
      obj["addr_type"].as_string().c_str() + "', prv_key = '" + obj["prv_key"].as_string().c_str() +
      "', pub_key = '" + obj["pub_key"].as_string().c_str() + "', wallet_id='" +
      std::to_string(obj["wallet_id"].as_uint64()) +
      "' WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";

  db_->Execute(query);

  return json::object();
}

json::object DbAddressManager::Delete(json::object obj) {
  std::string query =
      "DELETE FROM " + table_name_ + " WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";

  db_->Execute(query);
  return json::object();
}

json::object DbAddressManager::Select(json::object obj) {
  std::string query =
      "SELECT * FROM " + table_name_ + " WHERE id = " + std::to_string(obj["id"].as_uint64()) + ";";
  std::any res;
  db_->ExecuteWithResult(query, res);

  pqxx::result pq_res = std::any_cast<pqxx::result>(res);

  json::object res_obj;
  if (pq_res.size() > 0) {
    res_obj["drv_path"] = pq_res[0]["drv_path"].as<std::string>();
    res_obj["addr"] = pq_res[0]["addr"].as<std::string>();
    res_obj["addr_type"] = pq_res[0]["addr_type"].as<std::string>();
    res_obj["prv_key"] = pq_res[0]["prv_key"].as<std::string>();
    res_obj["pub_key"] = pq_res[0]["pub_key"].as<std::string>();
    res_obj["wallet_id"] = pq_res[0]["wallet_id"].as<uint32_t>();
  }

  return res_obj;
}