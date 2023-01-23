#include "db/db.h"

#include <string>
Db::Db() : db_name_("db") {}
Db::Db(std::string db_name) : db_name_(db_name) {}

void Db::SetDbName(std::string db_name) { db_name_ = db_name; }

std::string Db::GetDbName() { return db_name_; }