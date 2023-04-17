#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_set>

#include "absl/strings/str_join.h"
#include "pqxx/pqxx"
#include "secp256k1.h"
#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "third_party/gmpxx/gmpxx.h"

#include "bitcoin/address.h"
#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "bitcoin/json_rpc.h"
#include "bitcoin/privkey.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/rpc_tx.h"
#include "bitcoin/script.h"
#include "bitcoin/scriptpubkey.h"
#include "db/managers/address_manager.h"
#include "db/managers/transaction_manager.h"
#include "db/managers/wallet_manager.h"
#include "db/postgresql.h"
#include "main/global_vars.h"
#include "main/mempool_threads.h"
#include "main/thread_sync.h"
#include "net/comms.h"
#include "net/https.h"
#include "util/exceptions.h"
#include "util/util.h"

int main(int argc, char** argv) {
  GlobalVars& vars = GlobalVars::getInstance();
  ThreadSync threadSync;

  vars.db_ = new Postgresql("wallets_db", "localhost", 5432, "alkim", "alkim");

  vars.db_wallet_mgr = new DbWalletManager(vars.db_);
  vars.db_address_mgr = new DbAddressManager(vars.db_);
  vars.db_tx_mgr = new DbTransactionManager(vars.db_);

  if (vars.db_->IsConnected()) {
    std::cout << "Db Connected" << std::endl;
    vars.db_wallet_mgr->CreateTable();
    vars.db_address_mgr->CreateTable();
    vars.db_tx_mgr->CreateTable();

    std::thread mempool_thread(ListenMempool, std::ref(GlobalVars::getInstance()),
                               std::ref(threadSync));
    std::thread database_thread(WriteDatabase, std::ref(GlobalVars::getInstance()),
                                std::ref(threadSync));

    mempool_thread.join();
    database_thread.join();
  }

  return 0;
}
