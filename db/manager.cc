#include "db/manager.h"

DbManager::DbManager(Db* db, std::string table_name) : db_(db), table_name_(table_name) {}