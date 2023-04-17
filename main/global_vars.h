#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <queue>
#include <unordered_set>

#include "bitcoin/json_rpc.h"
#include "db/managers/address_manager.h"
#include "db/managers/transaction_manager.h"
#include "db/managers/wallet_manager.h"
#include "db/postgresql.h"

class GlobalVars {
 public:
  static GlobalVars& getInstance() {
    static GlobalVars instance;
    return instance;
  }

  // TODO: make std::unique_ptr / std::shared_ptr???
  //  database accessors
  Postgresql* db_;
  DbWalletManager* db_wallet_mgr;
  DbAddressManager* db_address_mgr;
  DbTransactionManager* db_tx_mgr;

  // keeps track of the mempool
  std::queue<std::vector<RpcTx>*>* mempool = new std::queue<std::vector<RpcTx>*>();

  // keeps track of the txids of the seen transactions
  std::unordered_set<std::string>* seen_txs = new std::unordered_set<std::string>();

  // indicates if it is the start of the application. if it is, then the first call to mempool
  // should check if the transactions already exist in the database
  bool is_start_mempool = true;

 private:
  GlobalVars(){};
  // avoid implicit generation of the copy constructor
  GlobalVars(const GlobalVars&) = delete;
  // avoid implicit copy assignment
  GlobalVars& operator=(const GlobalVars&) = delete;
};

#endif